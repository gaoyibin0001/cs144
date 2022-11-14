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
    int data_written_allow_num = data.size();
    string data_written;
    if ((_output.buffer_size() + (index-_current_index) + data.size()) > _capacity) {
          data_written_allow_num = min((_capacity - (_output.buffer_size() + (index-_current_index))), data.size()); 
    }
    // string un_a_value;
    if (eof) {
        // _got_end_index = true;
        _end_index = index+data.size();
    } 
    if (_current_index == _end_index) {
                _output.end_input();
                return;
              }

    if ((index <= _current_index) && ((index+data.size())> _current_index)) {
        data_written = data.substr((_current_index-index), data_written_allow_num);
        _output.write(data_written);
        if (eof) _output.end_input();
        _current_index += data_written.size();
        while(1){
            auto un_a_value = _un_assembled.upper_bound(_current_index); 
            if (un_a_value != _un_assembled.begin()) {
              --un_a_value;
              data_written = un_a_value->second.substr(_current_index-un_a_value->first);
              _output.write(data_written);
              
              _current_index += data_written.size();
              if (_current_index == _end_index) {
                _output.end_input();
                return;
              }

              while(un_a_value != _un_assembled.begin()) {
                _un_assembled.erase(un_a_value);
                _un_assembled_bytes -= un_a_value->second.size();
                --un_a_value;
              }
              _un_assembled.erase(un_a_value);
              _un_assembled_bytes -= un_a_value->second.size();

              
            } else{
                break;
            }
        }
    } else {
        // todo merge data list
        data_written = data.substr(0, data_written_allow_num);
        if (_un_assembled.count(index) == 0) {
           _un_assembled.insert({index, data_written});
           _un_assembled_bytes += data_written.size();
        }
        
    }

    

    // _push_string_action(data, index, eof);
}

size_t StreamReassembler::unassembled_bytes() const { return _un_assembled_bytes; }

bool StreamReassembler::empty() const { return _un_assembled_bytes == 0; }
