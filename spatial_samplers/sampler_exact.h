/*
Multiscale Universal Interface Code Coupling Library

Copyright (C) 2017 Y. H. Tang, S. Kudo, X. Bian, Z. Li, G. E. Karniadakis

This software is jointly licensed under the Apache License, Version 2.0
and the GNU General Public License version 3, you may use it according
to either.

** Apache License, version 2.0 **

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

** GNU General Public License, version 3 **

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

** File Details **

Filename: sampler_exact.h
Created: Feb 10, 2014
Author: Y. H. Tang
Description: Spatial sampler that provides a value at an exact point
             with no interpolation.
*/

#ifndef MUI_SAMPLER_EXACT_H_
#define MUI_SAMPLER_EXACT_H_

#include <limits>
#include "../config.h"
#include "../sampler.h"

namespace mui {

template<typename O_TP, typename I_TP=O_TP, typename CONFIG=default_config>
class sampler_exact {
public:
	using OTYPE      = O_TP;
	using ITYPE      = I_TP;
	using REAL       = typename CONFIG::REAL;
	using INT        = typename CONFIG::INT;
	using point_type = typename CONFIG::point_type;

	sampler_exact( REAL tol = std::numeric_limits<REAL>::epsilon() ) {
	    int exponent;
		frexp10<REAL>( std::numeric_limits<REAL>::max(), exponent );
		real_precision = static_cast<REAL>( exponent );
		tolerance = tol;
	}

	template<template<typename,typename> class CONTAINER>
	inline OTYPE filter( point_type focus, const CONTAINER<ITYPE,CONFIG> &data_points ) const {
		REAL point_tol = tolerance*real_precision;
		for( size_t i = 0 ; i < data_points.size() ; i++ ) {
			if ( norm( focus - data_points[i].first ) < point_tol ) {
				return data_points[i].second;
			}
		}

		//std::cerr << "MUI Warning [sampler_exact.h]: hit nothing\n";
		std::cout << "MUI sampler_exact: hit nothing" << std::endl << std::flush;

		std::cout << "Point tested: " << focus[0] << "," << focus[1] << "," << focus[2] << std::endl << std::flush;

		for( size_t i = 0 ; i < data_points.size() ; i++ ) {
			std::cout << "Point in data_points: " << data_points[i].first[0] << "," << data_points[i].first[1] << "," << data_points[i].first[2] << std::endl << std::flush;
		}

		return OTYPE(std::numeric_limits<REAL>::epsilon());
	}
	inline geometry::any_shape<CONFIG> support( point_type focus, REAL domain_mag ) const {
		return geometry::sphere<CONFIG>( focus, tolerance*domain_mag*real_precision );
	}

protected:
	    REAL tolerance;
	    REAL real_precision;
};

}

#endif /* MUI_SAMPLER_EXACT_H_ */
