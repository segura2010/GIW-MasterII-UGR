
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

    // initialize random seed:
    srand (time(NULL));

    Ratings myRatings;

    myRatings.readFile("./data/ua.base");

    std::cout << "Rating for user 1 movie 10: " << myRatings.getRating(1, 10) << std::endl;
    std::cout << "Average for user 1 movie: " << myRatings.getUserAverage(1) << std::endl;
    std::cout << "Readed users&movies: " << myRatings.getUsers() << " : " << myRatings.getMovies() << std::endl;

    Movies movies;
    movies.readFile("./data/u.item");
    std::cout << "Movie 3: "  << movies.getMovie(3) << std::endl;
    std::cout << "Readed movies: "  << movies.getMovies() << std::endl;

    // ratings for the user
    std::map<int, int> userRatings;
    for(int i=0;i<5;i++)
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
    std::cout << "Nearest: " << nearestUsers.size() << std::endl;

    std::priority_queue< std::pair<int, double>, std::vector< std::pair<int, double> >, RatingFunctor > recomendations;
    recomendations = myRatings.getUserRecomendations(userRatings, nearestUsers, movies);

    std::cout << "Best: " << recomendations.top().first << "(" << recomendations.top().second << ")" << std::endl;

}
