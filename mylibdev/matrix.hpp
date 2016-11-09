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

#include <vector>

namespace util
{
	template<class _T1>
	class matrix
	{
	private:
		std::vector<std::vector<_T1>> _matrix_data;

		size_t _max_rows, _max_column;
		float _scalar;
	public:
		matrix();
		matrix(size_t _row_arg, size_t _column_arg, float _scale);

		~matrix();

		bool set(size_t _row_arg, size_t _column_arg, _T1 _value);
		_T1 get(size_t _row_arg, size_t _column_arg);

		int return_rows() { return this->_max_rows; }
		int return_colums() { return this->_max_column; }

		void set_scale(float _scalar_arg) { this->_scalar = _scalar_arg; }
		float get_scale() { return this->_scalar; }

		/////////////////////////////////////////////////////////////

		void resize(size_t _row_arg, size_t _column_arg, float _scale){
			this->_max_rows = _row_arg;
			this->_max_column = _column_arg;
			this->_scalar = _scale;

			this->_matrix_data.resize(_row_arg);

			for (size_t i = 0; i < _row_arg; i++)
				this->_matrix_data[i].resize(_column_arg, 0);
		}

		/////////////////////////////////////////////////////////////
															
		matrix& operator*(_T1 _val_arg) {//					
			for (size_t i = 0; i < this->_max_rows; i++)	
				for (size_t j = 0; j < this->_max_column; j++)
					this->_matrix_data[i][j] *= _val_arg;	
															
			return *this;		
		}						
		matrix& operator*=(_T1 _val_arg) {
			for (size_t i = 0; i < this->_max_rows; i++)
				for (size_t j = 0; j < this->_max_column; j++)
					this->_matrix_data[i][j] *= _val_arg;

			return *this;
		}

		/////////////////////////////////////////////////////////////

		matrix& operator/(_T1 _val_arg) {				
			for (size_t i = 0; i < this->_max_rows; i++)
				for (size_t j = 0; j < this->_max_column; j++)
					this->_matrix_data[i][j] /= _val_arg;

			return *this;
		}
		matrix& operator/=(_T1 _val_arg) {
			for (size_t i = 0; i < this->_max_rows; i++)
				for (size_t j = 0; j < this->_max_column; j++)
					this->_matrix_data[i][j] /= _val_arg;

			return *this;
		}

		/////////////////////////////////////////////////////////////

		matrix& operator+(matrix& _matrix_arg) {
			if (this->_max_rows == _matrix_arg.return_rows() && this->_max_column == _matrix_arg.return_colums())
				for (size_t i = 0; i < this->_max_rows; i++)
					for (size_t j = 0; j < this->_max_column; j++)
						this->_matrix_data[i][j] += _matrix_arg.get(i, j);

			return *this;
		}
		matrix& operator+=(matrix& _matrix_arg) {
			if (this->_max_rows == _matrix_arg.return_rows() && this->_max_column == _matrix_arg.return_colums())
				for (size_t i = 0; i < this->_max_rows; i++)
					for (size_t j = 0; j < this->_max_column; j++)
						this->_matrix_data[i][j] += _matrix_arg.get(i, j);

			return *this;
		}

		/////////////////////////////////////////////////////////////

		matrix& operator-(matrix& _matrix_arg) {
			if (this->_max_rows == _matrix_arg.return_rows() && this->_max_column == _matrix_arg.return_colums())
				for (size_t i = 0; i < this->_max_rows; i++)
					for (size_t j = 0; j < this->_max_column; j++)
						this->_matrix_data[i][j] -= _matrix_arg.get(i, j);

			return *this;
		}
		matrix& operator-=(matrix& _matrix_arg) {
			if (this->_max_rows == _matrix_arg.return_rows() && this->_max_column == _matrix_arg.return_colums())
				for (size_t i = 0; i < this->_max_rows; i++)
					for (size_t j = 0; j < this->_max_column; j++)
						this->_matrix_data[i][j] -= _matrix_arg.get(i, j);

			return *this;
		}

		/////////////////////////////////////////////////////////////

		std::vector<_T1>& operator[](size_t _row_arg) {
			if (_row_arg >= 0 && this->_max_rows > _row_arg)
				return this->_matrix_data[_row_arg];
		}
	};

	/////////////////////////////////////////////////////////////

	template<class _T1>
	inline matrix<_T1>::matrix()
	{
	}

	/////////////////////////////////////////////////////////////

	template<class _T1>
	inline matrix<_T1>::matrix(size_t _row_arg, size_t _column_arg, float _scalar_arg)
	{
		this->_max_rows = _row_arg;
		this->_max_column = _column_arg;
		this->_scalar = _scalar_arg;

		this->_matrix_data.resize(_row_arg);

		for (size_t i = 0; i < _row_arg; i++)
			this->_matrix_data[i].resize(_column_arg, 0);
	}

	/////////////////////////////////////////////////////////////

	template<class _T1>
	inline matrix<_T1>::~matrix()
	{
	}

	/////////////////////////////////////////////////////////////

	template<class _T1>
	inline bool matrix<_T1>::set(size_t _row_arg, size_t _column_arg, _T1 _value)
	{
		//Index starts at 0 and ends at input-1
		if (
			_row_arg >= 0 && _max_rows > _row_arg &&
			_column_arg >= 0 && _max_column > _column_arg
			)
		{
			this->_matrix_data[_row_arg][_column_arg] = _value;
			return true;
		}
		else
			return false;
	}

	/////////////////////////////////////////////////////////////

	template<class _T1>
	inline _T1 matrix<_T1>::get(size_t _row_arg, size_t _column_arg)
	{
		if (
			_row_arg >= 0 && _max_rows > _row_arg &&
			_column_arg >= 0 && _max_column > _column_arg
			)
			return this->_matrix_data[_row_arg][_column_arg];
	}

	/////////////////////////////////////////////////////////////
}