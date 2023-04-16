#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    // DUMMY_CODE(seg);
    if (seg.header().syn) {
        syn_ = true;
        isn_ = seg.header().seqno;
    }
    if (syn_) {
        WrappingInt32 begin_seqno = seg.header().seqno;
        if (seg.header().syn) {
            begin_seqno = begin_seqno + 1;
        }
        _reassembler.push_substring(seg.payload().copy(),
                                    unwrap(begin_seqno, isn_, _reassembler.stream_out().bytes_written()) - 1,
                                    seg.header().fin);
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (!syn_) {
        return {};
    }
    WrappingInt32 to_ack_num = wrap(_reassembler.stream_out().bytes_written() + 1, isn_);  // todo bug
    if (_reassembler.stream_out().input_ended()) {
        to_ack_num = to_ack_num + 1;
    }
    return to_ack_num;
}

size_t TCPReceiver::window_size() const { return _reassembler.stream_out().remaining_capacity(); }
