/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 22:35:15 by ellanglo          #+#    #+#             */
/*   Updated: 2025/05/26 04:16:50 by wirare           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#define SETGET(type, _name, Name) 											\
	public: 																\
		inline void set##Name(const type &value) { this->_name = value; }	\
		inline type get##Name(void) const { return this->_name; }			\
	private:																\
		type _name;
		