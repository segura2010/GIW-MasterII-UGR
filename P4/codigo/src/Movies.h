
#ifndef MOVIES_H
#define MOVIES_H


// To read files
#include <fstream>

#include <stdio.h>
#include <iostream>

#include <vector>
#include <map>


class Movies
{

private:
    std::map<int, std::string> movies;
    int numMovies;

public:

    Movies()
    {
        numMovies = 0;
    }

    void readFile(std::string filename)
	{
		std::fstream file(filename.c_str(), std::ios_base::in);

		if(!file)
		{
			throw std::runtime_error("Error: Invalid filename");
		}

        int movieid = 0;
        char actChar[512];
        std::string name;

		do{

            file >> movieid;
            file.getline(actChar, 512);

            std::string str(actChar);
            str = str.substr(1); // delete first |
            name = str.substr( 0, str.find("|") ); // to end name

            movies[movieid] = name; // save movie
            numMovies++;

            //std::cout << "MID: " << movieid << " NAME: " << name << std::endl;

        }while(!file.eof());
	}

    std::string getMovie(int mid)
    {
        return movies[mid];
    }

    int getMovies()
    {
        return numMovies;
    }

    std::map<int, std::string>::iterator begin()
    {
        return movies.begin();
    }

    std::map<int, std::string>::iterator end()
    {
        return movies.end();
    }

};

#endif
