
#include <map>
#include <iostream>
#include <cassert>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Ratings.h"
#include "Movies.h"

int random(int lower, int upper)
{
    return rand() % upper + lower;
}

int main(int argc, char **argv)
{

    if( argc < 3 )
	{
		std::cout << "Usage: " << argv[0] << " ratings_file movies_file " << std::endl;
        std::cout << "Example: " << argv[0] << " ./data/ua.base ./data/u.item " << std::endl;
		exit(0);
	}

    // initialize random seed:
    srand (time(NULL));

    Ratings myRatings;

    myRatings.readFile(argv[1]);

    //std::cout << "Rating for user 1 movie 10: " << myRatings.getRating(1, 10) << std::endl;
    //std::cout << "Average for user 1 movie: " << myRatings.getUserAverage(1) << std::endl;
    std::cout << "Readed users & movies ratings: " << myRatings.getUsers() << " : " << myRatings.getMovies() << std::endl;

    Movies movies;
    movies.readFile(argv[2]);
    //std::cout << "Movie 3: "  << movies.getMovie(3) << std::endl;
    std::cout << "Readed movies: "  << movies.getMovies() << std::endl;

    // ratings for the user
    std::map<int, int> userRatings;
    for(int i=0;i<20;i++)
    {
        int r = random(1, movies.getMovies()); // get random movie to request rating
        int rating = 0;
        do{
            std::cout << "Que calificación le das a la película: " << movies.getMovie(r) << " ";
            std::cin >> rating;
        }while( rating > 5 || rating < 0 );
        userRatings[r] = rating; // save rating
    }

    std::map<int, double > nearestUsers;
    nearestUsers = myRatings.getPearsonNearestUsers(userRatings, 3);

    std::priority_queue< std::pair<int, double>, std::vector< std::pair<int, double> >, RatingFunctor > recommendations;
    recommendations = myRatings.getUserRecommendations(userRatings, nearestUsers, movies);


    std::cout << "============================" << std::endl;
    std::cout << "Recommedations: " << std::endl;
    std::cout << "============================" << std::endl;
    int maxRecommendations = 10, movieid = 0;
    double rating = 0.0;
    std::string movieName;
    for(int i=0;i<maxRecommendations && !recommendations.empty();i++)
    {
        movieid = recommendations.top().first;
        rating = recommendations.top().second;
        movieName = movies.getMovie(movieid);
        std::cout << "#" << i << ". " << movieName << " (" << rating << ")" << std::endl;
        recommendations.pop();
    }

}
