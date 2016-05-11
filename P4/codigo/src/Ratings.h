
#ifndef RATINGS_H
#define RATINGS_H

// To read files
#include <fstream>

#include <stdio.h>
#include <iostream>
#include <math.h>

#include <vector>
#include <map>
#include <queue>

#include "Movies.h"


// Functor for PriorityQueue
class RatingFunctor
{
    bool reverse;
    public:
    RatingFunctor(const bool& revparam=false)
    {
        reverse=revparam;
    }
    bool operator() (const std::pair<int, double>& lhs, const std::pair<int, double>&rhs) const
    {
        if (reverse)
            return (lhs.second > rhs.second);
        else
            return (lhs.second < rhs.second);
    }
};



class Ratings
{

private:
    std::map<int, std::map<int, int> > ratings;
    std::map<int, double> average; // rating average for each user
    int numUsers, numMovies;

public:

    Ratings()
    {
        numUsers = 0; numMovies = 0;
    }

    void readFile(std::string filename)
	{
		std::fstream file(filename.c_str(), std::ios_base::in);

		if(!file)
		{
			throw std::runtime_error("Error: Invalid filename");
		}


        int userid = 0, movieid = 0, rating = 0, randomNumber = 0;

		do{

            // format is: userid movieid rating randomnumber
            file >> userid;
            file >> movieid;
            file >> rating;
            file >> randomNumber;

            //std::cout << "UID: " << userid << " MID: " << movieid << " RAT: " << rating << std::endl;

            std::map<int, std::map<int, int> >::iterator userit = ratings.find( userid );
            if( userit != ratings.end() )
            {   // if user exists put movie with rating
                (userit->second)[movieid] = rating;
                numMovies++;
                average[userid] = (double)average[userid] + (double)rating; // add rating to the average
            }
            else
            {   // if not, add a new user with his rating
                ratings[userid] = std::map<int, int>();
                ratings[userid][movieid] = rating;
                numUsers++; numMovies++;
                average[userid] = (double)rating; // initilize average
            }


        }while(!file.eof());

        // Calculate average
        std::map<int, double>::iterator it;
        for(it=average.begin();it!=average.end();it++)
        {
            it->second = (double)it->second / (double)ratings[it->first].size();
        }

	}

    int getRating(int uid, int mid)
    {
        return ratings[uid][mid];
    }

    double getUserAverage(int uid)
    {
        return average[uid];
    }

    int getUsers()
    {
        return numUsers;
    }

    int getMovies()
    {
        return numMovies;
    }

    /*
        it receives user ratings for movies and returns the K nearest users
    */
    std::map<int, double > getPearsonNearestUsers(std::map<int, int> userRatings, int k)
    {
        std::map<int, double > nearestUsers;

        std::map<int, std::map<int, int> >::iterator userit;
        std::map<int, int>::iterator it;

        // PAIR IDUSER and SIMILITUDE, using vector as internal rep and my functor to sort
        std::priority_queue< std::pair<int, double>, std::vector< std::pair<int, double> >, RatingFunctor > pq;

        // first: calculate user average
        double userAvg = 0.0;
        for(it=userRatings.begin();it!=userRatings.end();it++)
        {
            userAvg += it->second;
        }
        userAvg = userAvg / userRatings.size();

        for(userit=ratings.begin();userit!=ratings.end();userit++)
        {
            double similitude = 0.0;
            double numerator = 0.0;
            double otherUserDenominator = 0.0, userDenominator = 0.0;
            for(it=userRatings.begin();it!=userRatings.end();it++)
            {
                int movieid = it->first;
                int actUserRating = it->second;
                int actOtherUserRating = userit->second[movieid];
                double otherUserAvg = average[userit->first];

                numerator = numerator + ( (actOtherUserRating-otherUserAvg)*(actUserRating-userAvg) );
                otherUserDenominator = otherUserDenominator + ( (actOtherUserRating-otherUserAvg)*(actOtherUserRating-otherUserAvg) );
                userDenominator = userDenominator + ( (actUserRating-userAvg)*(actUserRating-userAvg) );
            }

            otherUserDenominator = sqrt(otherUserDenominator);
            userDenominator = sqrt(userDenominator);

            similitude = numerator / ( otherUserDenominator*userDenominator );
            //std::cout << "USERID: " << userit->first << ": " << similitude << std::endl;

            // now, we must sort results, for example using a priority queue with our own functor
            // http://www.cplusplus.com/reference/queue/priority_queue/priority_queue/
            pq.push( std::pair<int, double>(userit->first, similitude) );
        }

        // prepare output
        int i = 0;
        while( !pq.empty() && i<k )
        {
            std::pair<int, double> act = pq.top(); pq.pop();
            nearestUsers[act.first] = act.second; // save user for the output
            i++;

            //std::cout << "USERID: " << act.first << ": " << act.second << std::endl;
        }


        return nearestUsers;
    }

    /*
        it receives K nearest neightbours, user ratings and movies
        then it calculates rating's predictions for not watched movies and returns them
    */
    std::priority_queue< std::pair<int, double>, std::vector< std::pair<int, double> >, RatingFunctor > getUserRecomendations(const std::map<int, int>& userRatings, std::map<int, double>& nearestUsers, Movies& movies)
    {
        /* TODO:
            Check if is not watched!!
            Check if users watched movie!!
        */
        std::priority_queue< std::pair<int, double>, std::vector< std::pair<int, double> >, RatingFunctor > predictedRatings;

        double predictedRating = 0.0;

        std::map<int, std::string>::iterator moviesIt;
        std::map<int, double>::iterator usersIt;
        for(moviesIt=movies.begin();moviesIt!=movies.end();moviesIt++)
        {
            predictedRating = 0.0;
            for(usersIt=nearestUsers.begin();usersIt!=nearestUsers.end();usersIt++)
            {
                int movieRatingByUser = ratings[usersIt->first][moviesIt->first];
                predictedRating = predictedRating + ( movieRatingByUser*usersIt->second );
            }
            predictedRating = predictedRating / nearestUsers.size();

            predictedRatings.push( std::pair<int, double>(moviesIt->first, predictedRating) );
        }

        return predictedRatings;
    }

};

#endif
