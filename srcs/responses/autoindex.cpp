/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 11:57:16 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/04 19:48:49 by pohl             ###   ########.fr       */
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

// The autoindex function return a string with an automatic index create in HTML
// Root is the directory where the autoindex have to function

std::string autoIndex(std::string	root)
{
	DIR*			dirp;
	struct dirent*	direntp;
	std::string		index;
	root = "/mnt/nfs/homes/fmonbeig/Project/webserv"; // Enlever quand on aura le lien vers le dossier

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
	}
	return index;
}
