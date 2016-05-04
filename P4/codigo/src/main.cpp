
#include <map>
#include <iostream>
#include <cassert>

#include "Ratings.h"
#include "Movies.h"

int main(int argc, char **argv)
{

    Ratings myRatings;

    myRatings.readFile("./data/ua.base");

    std::cout << "Rating for user 1 movie 10: " << myRatings.getRating(1, 10) << std::endl;
    std::cout << "Readed users&movies: " << myRatings.getUsers() << " : " << myRatings.getMovies() << std::endl;

    Movies movies;
    movies.readFile("./data/u.item");
    std::cout << "Movie 3: "  << movies.getMovie(3) << std::endl;
    std::cout << "Readed movies: "  << movies.getMovies() << std::endl;

}
