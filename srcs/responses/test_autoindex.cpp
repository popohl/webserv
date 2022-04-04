/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index_test.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 11:57:16 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/01 14:20:46 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

static void	createLink(std::string & index, std::string & root, struct dirent *direntp)
{
	index += "<a href=\"";
	// index += root;
	// index += "/";
	index += direntp->d_name;
	index += "\">";
	index += direntp->d_name;
	index += "</a>";
}

int main( void )
{
	DIR*			dirp;
	struct dirent*	direntp;
	std::string		index;
	std::string		root = "/mnt/nfs/homes/fmonbeig/Project/webserv";

	dirp = opendir(root.c_str());
	index += "<!DOCTYPE html>\n<html>\n\n<title>INDEX</title>\n\n<h1>INDEX</h1>";
	if( dirp != NULL )
	{
        for(;;)
		{
            direntp = readdir( dirp );
            if ( direntp == NULL )
				break;
			index += "<h4>";
			if ( direntp->d_type == DT_DIR)
				index += "__Directory__| ";
			else if ( direntp->d_type == DT_REG)
				index += "_Regular file_| ";
			else
				index += "                ";
			createLink(index, root, direntp);
			index += "</h4>\n";
		}
		index += "</html>";
		closedir( dirp );
		std::ofstream	outfile("INDEX.html");
		if (outfile.fail())
		{
			std::cerr << "Can't create the file\n";
			return EXIT_FAILURE;
		}
		outfile << index << std::endl;
		outfile.close();
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
