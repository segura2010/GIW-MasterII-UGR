

// To read files
#include <fstream>

#include <stdio.h>
#include <iostream>
#include <math.h>

#include <vector>
#include <map>


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
    std::map<int, std::map<int, int> > getPearsonNearestUsers(std::map<int, int> userRatings, int k)
    {
        std::map<int, std::map<int, int> > nearestUsers;

        std::map<int, std::map<int, int> >::iterator userit;
        std::map<int, int>::iterator it;

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
            // Not finished!!
            // now, we must sort results, for example using a priority queue with our own functor
            // http://www.cplusplus.com/reference/queue/priority_queue/priority_queue/
        }

        return nearestUsers;
    }

};
