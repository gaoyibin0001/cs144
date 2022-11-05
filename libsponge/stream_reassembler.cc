#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

// template <typename... Targs>
// void StreamReassembler::_push_string_action(const std::string &data, const uint64_t index, const bool eof) {
//     if (index == _current_index) {
//         _capacity += data.size();
//         _output.write(data);
//         _current_index += 1;
//     } 
// }


using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(ByteStream(capacity)), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (((_output.buffer_size() + _un_assembled_bytes + data.size()) > _capacity)) {
        return;
    }
    // string un_a_value;

    if (index == _current_index) {
        _output.write(data);
        if (_current_index == _end_index) _output.end_input();
        _current_index += 1;
        while(1){
            auto un_a_value = _un_assembled.find(_current_index); 
            if (un_a_value != _un_assembled.end()) {
              _output.write(un_a_value->second);
              if (un_a_value->first == _end_index) _output.end_input();
              _current_index += 1;
              _un_assembled.erase(un_a_value);
              _un_assembled_bytes -= un_a_value->second.size();
            } else{
                break;
            }
        }
    } else {
        if (_un_assembled.count(index) == 0) {
           _un_assembled.insert({index, data});
           _un_assembled_bytes += data.size();
        }
        
    }

    if (eof) {
        // _got_end_index = true;
        _end_index = index;
    } 

    // _push_string_action(data, index, eof);
}

size_t StreamReassembler::unassembled_bytes() const { return _un_assembled_bytes; }

bool StreamReassembler::empty() const { return _un_assembled_bytes == 0; }
