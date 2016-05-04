
#include <map>
#include <iostream>
#include <cassert>

#include "Ratings.h"

int main(int argc, char **argv)
{

    Ratings myRatings;

    myRatings.readFile("./data/ua.base");

    std::cout << myRatings.getRating(1, 10) << std::endl;
    std::cout << myRatings.getUsers() << " : " << myRatings.getMovies() << std::endl;

}
