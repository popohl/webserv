/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:31:24 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/21 13:20:52 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>

int main()
{
	std::vector<int> temp;

	std::cout << &temp << std::endl;

	temp.push_back(10);
	temp.push_back(20);
	temp.push_back(30);

	for(int i = 0; i < temp.size(); i++)
	{
		std::cout << temp.data() + i << std::endl;
	}
}
