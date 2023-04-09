#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

// template <typename... Targs>
// void StreamReassembler::_push_string_action(const std::string &data, const uint64_t index, const bool eof) {
//     if (index == current_index_) {
//         _capacity += data.size();
//         _output.write(data);
//         current_index_ += 1;
//     } 
// }


using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(ByteStream(capacity)), _capacity(capacity) {
  // un_index_.push_back({0,0});
  // un_assembled_.push_back("");
}

void StreamReassembler::push_unassembled_substring_(const std::string &data, const uint64_t begin_index, const uint64_t end_index) {
    if (un_assembled_.size() == 0) {
        un_assembled_.push_back(data);
        un_index_.push_back({begin_index, end_index});
        un_assembled_bytes_ += data.size();
        return;
    }
    size_t i = 0;
    uint64_t current_write_end_index = end_index;
    uint64_t current_write_begin_index = begin_index;
  
    for (i = 0; i < un_assembled_.size(); i++) {
        if (un_index_[i].first > begin_index) {
            current_write_end_index = min(un_index_[i].first, current_write_end_index);
            break;
        }
        if (un_index_[i].second > begin_index) {
           current_write_begin_index = un_index_[i].second;
           // data is already in un_assembled_
           if (current_write_begin_index >= end_index){
             return;
           }
           i++;
           if (un_assembled_.size()>i && un_index_[i].first < end_index) {
              current_write_end_index = un_index_[i].first;
            }
           break;
        }
    }

    string data_written = data.substr(current_write_begin_index-begin_index, current_write_end_index-current_write_begin_index);
    un_assembled_.insert(un_assembled_.begin()+i, data_written);
    un_index_.insert(un_index_.begin()+i, {current_write_begin_index, current_write_end_index});
    un_assembled_bytes_ += data_written.size();

    if (current_write_end_index < end_index) {
        push_unassembled_substring_(data.substr(current_write_end_index-begin_index), current_write_end_index, end_index);
    }
    
}

void StreamReassembler::pop_unassembled_to_output_(){
    if (un_index_.size()==0) {
      return;
    }
    if (un_index_[0].first != current_index_) {
      return;
    }

    _output.write(un_assembled_[0]);
    current_index_ = un_index_[0].second;
    un_assembled_bytes_ -= un_assembled_[0].size();

    un_assembled_.erase(un_assembled_.begin());
    un_index_.erase(un_index_.begin());
    
    pop_unassembled_to_output_();
}


void StreamReassembler::update_end_eof_() {
  // todo maybe greater than???
  if (current_index_ == end_index_) {
        _output.end_input();
        return;
    }
}
//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // int data_written_allow_num = data.size();
    // string data_written;
    // // index - current_index： meaning the relative gap distance of index
    // if ((_output.buffer_size() + (index-current_index_) + data.size()) > _capacity) {
    //       data_written_allow_num = min((_capacity - (_output.buffer_size() + (index-current_index_))), data.size()); 
    // }
    // string un_a_value;
    
    uint64_t current_data_index_end = index + data.size(); 
    if (eof) {
        end_index_ = current_data_index_end;
        update_end_eof_();
    }
    
    if (data.size() == 0) return;
    // data end index is smaller than current index
    if (current_data_index_end <= current_index_) {
        return;
    }
    
    // current max data index allowed
    uint64_t current_max_index = current_index_ + (_capacity - _output.buffer_size());
    current_data_index_end = min(current_data_index_end, current_max_index);

    uint64_t current_data_index_begin = max(index, current_index_);
    // data to write out of bound
    if(current_data_index_begin > current_data_index_end) {
        return;
    }
    push_unassembled_substring_(data.substr(current_data_index_begin-index, current_data_index_end-current_data_index_begin), current_data_index_begin, current_data_index_end);
    pop_unassembled_to_output_();
    
    update_end_eof_();
    return;

    

    // // 是否要覆盖过去的数据？现在覆盖！！！
    // if ((index <= current_index_) && ((index+data.size())> current_index_)) {
    //     data_written = data.substr((current_index_-index), data_written_allow_num);
    //     _output.write(data_written);
    //     if (eof) _output.end_input();
    //     current_index_ += data_written.size();
    //     auto un_a_value = un_assembled_.upper_bound(current_index_); 
    //     if (un_a_value != un_assembled_.begin()) {
    //       --un_a_value;
    //       if ((un_a_value->first + un_a_value->second.size()) > current_index_) {
    //           data_written = un_a_value->second.substr(current_index_-un_a_value->first);
    //           _output.write(data_written);
          
    //         current_index_ += data_written.size();
    //         if (current_index_ == end_index_) {
    //           _output.end_input();
    //           return;
    //       }
    //       }
          

    //       while(un_a_value != un_assembled_.begin()) {
    //         un_assembled_.erase(un_a_value);
    //         un_assembled_bytes_ -= un_a_value->second.size();
    //         --un_a_value;
    //       }
    //       un_assembled_.erase(un_a_value);
    //       un_assembled_bytes_ -= un_a_value->second.size();

    //     }  
    // } else {
    //     // todo merge data list
    //     auto lower_value = un_assembled_.lower_bound(index); 
    //         if (lower_value != un_assembled_.begin()) {
    //           --lower_value;
    //           if ((lower_value->first + lower_value->second.size()) >= index){
    //             lower_value->second += data;
    //           }
              
    //         }
              


    //     data_written = data.substr(0, data_written_allow_num);
    //     if (un_assembled_.count(index) == 0) {
    //        un_assembled_.insert({index, data_written});
    //        un_assembled_bytes_ += data_written.size();
    //     }
        
    // }

    

    // _push_string_action(data, index, eof);
}

size_t StreamReassembler::unassembled_bytes() const { return un_assembled_bytes_; }

bool StreamReassembler::empty() const { return un_assembled_bytes_ == 0; }
