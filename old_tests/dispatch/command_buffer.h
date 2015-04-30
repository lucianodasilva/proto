#ifndef _dispatch_command_buffer_h_
#define _dispatch_command_buffer_h_

#include "abstract.h"

#include <algorithm>
#include <cinttypes>

namespace dispatch {

	template < class _t, class _k = command_key >
	struct command_buffer {
	private:

		struct item_t {
			_k key;
			_t data;
		};

		static const uint32_t radix_count = sizeof(_k);
		static const uint32_t table_size = 256 * radix_count;

		size_t		_capacity;
		size_t		_size;

		item_t *	_data;
		item_t *	_data_swap_buffer;

		uint32_t	_histograms[table_size];
		uint32_t	_offsets[table_size];

		uint32_t *	_hr[radix_count];
		uint32_t *	_or[radix_count];

	public:

		inline size_t size() { return _size; }
		inline size_t capacity() { return _capacity; }

		inline void clear() {
			_size = 0;
		}

		inline command_buffer() :
			_capacity(0), _size(0), _data(nullptr), _data_swap_buffer(nullptr)
		{
			// setup shortcuts
			for (int i = 0; i < radix_count; ++i) {
				_hr[i] = &_histograms[256 * i];
				_or[i] = &_offsets[256 * i];
			}
		}

		inline virtual ~command_buffer() {
			delete [] _data;
			delete [] _data_swap_buffer;
		}

		inline void reserve(size_t cap) {

			if (cap < _capacity)
				return;

			item_t * _new_data = new item_t[cap];
			item_t * _new_data_swap = new item_t[cap];

			std::copy(_data, _data + _size, _new_data);

			_capacity = cap;

			delete [] _data;
			delete [] _data_swap_buffer;

			_data = _new_data;
			_data_swap_buffer = _new_data_swap;

		}

		inline void push_back(const _k & key, const _t & value ) {
			if (_size == _capacity) {
				reserve(_capacity * 2);
			}

			item_t & item = _data[_size];

			item.key = key;
			item.data = value;

			++_size;
		}

		inline _t & operator [] ( size_t index ) {
			return _data[index].data;
		}

		inline void sort() {
			uint8_t value;
			uint8_t * p_value;

			item_t * swap_tmp;

			// calculate histograms
			std::fill(_histograms, _histograms + table_size, 0);

			for (size_t i = 0; i < _size; ++i) {
				p_value = (uint8_t *)(_data + i);
			
				for (size_t j = 0; j < radix_count; ++j)
					++_hr[j][p_value[j]];
			}

			for (size_t i = 0; i < table_size; ++i) {
				if (i % 256 != 0)
					_offsets[i] = _offsets[i - 1] + _histograms[i - 1];
				else
					_offsets[i] = 0;
			}

			// swap stuff around
			for (uint32_t radix = 0; radix < radix_count; ++radix) {
				// copy to offseted positions
				for (uint32_t i = 0; i < _size; ++i) {
					item_t & item = _data[i];

					value = ((uint8_t *)(&item.key))[radix];
					_data_swap_buffer[_or[radix][value]++] = item;
				}

				// swap
				swap_tmp = _data;
				_data = _data_swap_buffer;
				_data_swap_buffer = swap_tmp;
			}
		}

	};

}

#endif