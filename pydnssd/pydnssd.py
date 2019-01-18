import socket
import random

class LlmnrHeader(object):
    def __init__(self, query_id=random.getrandbits(16)):
        self.query_id = query_id # ID
        self.is_response = is_response   # QR
        self.op_code = op_code

# https://tools.ietf.org/html/rfc4795#page-5

class LlmnrHeader(object):
    query_id = 0
    is_response = False
    has_conflict = False
    trucated = False
    tentative = False
    response_code = 0
    question_count = 1
    answer_count = 0
    ns_count = 0
    additional_count = 0

    _FORMAT = '>HBBHHHH'

    def to_bytes(self):
        qr_opcode_c_tc_t = (int(self.is_response) << 0 |
                            int(self.has_conflict) << 5 |
                            int(self.trucated) << 6 |
                            int(self.tentative) << 7)
        response_code = self.response_code << 4;

        return struct.pack(_FORMAT, self.query_id, qr_opcode_c_tc_t, response_code,
                           self.question_count, self.answer_count, self.ns_count,
                           self.additional_count)

    def from_bytes(b):
        ret = LlmnrHeader()
        (ret.query_id, qr_opcode_c_tc_t, response_code,
         ret.question_count, ret.answer_count, ret.ns_count,
         ret.additional_count) = struct.unpack(_FORMAT, b)

        ret.is_response  = bool(qr_opcode_c_tc_t & (1 << 0))
        ret.has_conflict = bool(qr_opcode_c_tc_t & (1 << 5))
        ret.trucated     = bool(qr_opcode_c_tc_t & (1 << 6))
        ret.tentative    = bool(qr_opcode_c_tc_t & (1 << 7))
        ret.response_code = response_code >> 4
        return ret

class DnsMessageQuestion(object):
    TYPE_A = 1
    TYPE_CNAME = 5
    TYPE_PTR = 12
    TYPE_SRV = 33
    QTYPE_ALL = 255
    TYPE_AAAA = 28 # https://tools.ietf.org/html/rfc3596#section-2.1
    CLASS_IN = 1 # INternet

    name_parts = [b'example', b'com', b'']
    type_ = QTYPE_ALL
    class_ = CLASS_IN

    def to_bytes(self):
        b = []
        for x in self.name_parts:
            b += [len(x).to_bytes(1), x]
        b += [struct.pack('>HH', self.type_, self.class_)]
        return b''.join(b)

    def from_bytes(b):
        ret = DnsMessageQuestionSection()
        ret.name_parts = []
        pos = 0
        while True:
            slen = b[pos]
            pos += 1
            if slen == 0:
                break
            ret.name_parts.append(b[pos:pos+slen])
            pos += slen
        (ret.type_, ret.class_) = struct.unpack('>HH', b[pos:pos+4])
        pos += 4
        assert len(b) == pos
        return ret

class DsnMessageRecord(object):
    name = [b'example', b'com', b'']
    type_ = TYPE_A
    class_ = CLASS_IN
    ttl = 0
    data = b''


def llmnr_encode(x):
    query_id = x['id']
    is_response = x[
    assert x.get('OPCODE', 1)
    assert x.get('QDCOUNT', 1)


class DnsSdService(object):
    def __init__(