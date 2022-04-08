/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 11:57:16 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/08 12:28:57 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <unistd.h>

static void	createLink(std::string & index, struct dirent *direntp)
{
	index += "<a href=\"";
	index += direntp->d_name;
	if (direntp->d_type == DT_DIR)
		index += "/";
	index += "\">";
	index += direntp->d_name;
	index += "</a>";
}

// The autoindex function return a string with an automatic index create in HTML
// Root is the directory where the html page are (ex: www)
std::string autoIndex(std::string filePath)
{
	DIR*			dirp;
	struct dirent*	direntp;
	std::string		index;
	char			buffer[PATH_MAX];
	std::string		path;

	path = getcwd(buffer, PATH_MAX);
	path += "/";
	path += filePath;
	dirp = opendir(path.c_str());
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
			createLink(index, direntp);
			index += "</h4>\n";
		}
		index += "</html>";
		closedir( dirp );
	}
	return index;
}
