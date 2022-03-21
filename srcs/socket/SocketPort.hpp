/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPort.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/21 15:49:52 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKECTPORT_HPP
#define SOCKECTPORT_HPP

#include "ASocket.hpp"

class SocketPort : public ASocket
{
	protected:
		SocketPort();

	public:

		SocketPort(int const & port);
		SocketPort(SocketPort const & other );
		virtual ~SocketPort();

		SocketPort & operator=(SocketPort const & other);

};

#endif
