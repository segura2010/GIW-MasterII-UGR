

// To read files
#include <fstream>

#include <stdio.h>
#include <iostream>

#include <vector>
#include <map>


class Ratings
{

private:
    std::map<int, std::map<int, int> > ratings;
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
            }
            else
            {   // if not, add a new user with his rating
                ratings[userid] = std::map<int, int>();
                ratings[userid][movieid] = rating;
                numUsers++; numMovies++;
            }

        }while(!file.eof());
	}

    int getRating(int uid, int mid)
    {
        return ratings[uid][mid];
    }

    int getUsers()
    {
        return numUsers;
    }

    int getMovies()
    {
        return numMovies;
    }

};
