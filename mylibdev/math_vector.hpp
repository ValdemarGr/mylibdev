#pragma once

//##START#####TEXT#####START##
///     DATE:	2/05/2016
///     TIME:	23:30
///     NAME:	Samuel Valdemar Grange
///     
///     FUNCTIONALITY:
///		matrix, addition, 
///     
///     DESCRIPTION:
///		Provides a class for matrix calculations
///     
//##END#####TEXT#####END##

#define _USE_MATH_DEFINES

#include <cmath>
#include <math.h>

namespace util {
	template<class _T1>
	class math_vector
	{
	private:
		_T1 _x_pos, _y_pos;
		float _length, _rot;
	public:
		math_vector();
		~math_vector();

		void create_polar(float _length_arg, float _rot_arg);
		void create_cartesian(_T1 _x_pos_arg, _T1 _y_pos_arg);

		float len();
		float angle();
		
		_T1 return_x() { return this->_x_pos; }
		_T1 return_y() { return this->_y_pos; }

		float return_len() { return this->_length; }
		float return_rot() { return this->_rot; }
	};


	template<class _T1>
	inline math_vector<_T1>::math_vector()
	{
	}

	template<class _T1>
	inline math_vector<_T1>::~math_vector()
	{
	}

	template<class _T1>
	inline void math_vector<_T1>::create_polar(float _length_arg, float _rot_arg)
	{
		this->_length = _length_arg;
		this->_rot = _rot_arg;

		this->_x_pos = _length_arg * std::cos(_rot_arg * M_PI / 180);
		this->_y_pos = _length_arg * std::sin(_rot_arg * M_PI / 180);
	}

	template<class _T1>
	inline void math_vector<_T1>::create_cartesian(_T1 _x_pos_arg, _T1 _y_pos_arg)
	{
		this->_x_pos = _x_pos_arg;
		this->_y_pos = _y_pos_arg;

		this->_length = len();
		this->_rot = angle() * -1 + 90;
	}

	template<class _T1>
	inline float math_vector<_T1>::len()
	{
		return std::sqrt(std::pow(this->_x_pos, 2) + std::pow(this->_y_pos, 2));
	}

	template<class _T1>
	inline float math_vector<_T1>::angle()
	{
		return (std::atan2(this->_x_pos, this->_y_pos) * 180 / M_PI);
	}

}