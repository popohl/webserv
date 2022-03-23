/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FdSet.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/23 15:06:37 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDSET_HPP
#define FDSET_HPP

      #include <sys/time.h>
       #include <sys/types.h>
       #include <unistd.h>

class FdSet
{
	protected:
		fd_set	_set;

	public:
		FdSet();
		FdSet(FdSet const & other);
		virtual ~FdSet();

		FdSet & operator=(FdSet const & other);

		fd_set	getset();

		void	add(int fd);
		void	remove(int fd);
		void	clearAll();
		bool	isset(int fd);

};

#endif
