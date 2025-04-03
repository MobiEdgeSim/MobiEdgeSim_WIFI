import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;


class TimestampedPacket extends inet::FieldsChunk
{
    simtime_t sendTime;
    int sequenceNumber;
    unsigned int payloadSize; 
}
