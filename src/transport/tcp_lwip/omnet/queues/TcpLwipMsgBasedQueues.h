//
// Copyright (C) 2004 Andras Varga
//               2010 Zoltan Bojthe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_TcpLwipMSGBASEDQUEUES_H
#define __INET_TcpLwipMSGBASEDQUEUES_H

#include <omnetpp.h>

#include "TcpLwipQueues.h"

#include "TCPConnection.h"

/**
 * Send/Receive queue that manages "virtual bytes", that is, byte counts only.
 */

class INET_API TcpLwipMsgBasedSendQueue : public TcpLwipSendQueue
{
  protected:
    struct Payload
    {
        unsigned int endSequenceNo;
        cPacket *msg;
    };
    typedef std::list<Payload> PayloadQueue;
    PayloadQueue payloadQueueM;

    uint32 beginM;  // 1st sequence number stored
    uint32 endM;    // last sequence number stored +1
    bool isValidSeqNoM;
    long int unsentTcpLayerBytesM;

  public:
    /**
     * Ctor.
     */
    TcpLwipMsgBasedSendQueue();

    /**
     * Virtual dtor.
     */
    virtual ~TcpLwipMsgBasedSendQueue();

    /**
     * set connection queue.
     */
    virtual void setConnection(TcpLwipConnection *connP);

    /**
     * Called on SEND app command, it inserts in the queue the data the user
     * wants to send. Implementations of this abstract class will decide
     * what this means: copying actual bytes, just increasing the
     * "last byte queued" variable, or storing cMessage object(s).
     * The msg object should not be referenced after this point (sendQueue may
     * delete it.)
     */
    virtual void enqueueAppData(cPacket *msgP);

    /**
     * Copy data to the buffer for send to LWIP.
     * returns lengh of copied data.
     * create msg for socket->send_data()
     *
     * called before called socket->send_data()
     */
//    virtual int getTcpLayerMsg(void* bufferP, int bufferLengthP);
    virtual int getBytesForTcpLayer(void* bufferP, int bufferLengthP);

    /**
     * Remove msgLengthP bytes from NSCqueue
     *
     * called with return value of socket->send_data() if larger than 0
     */
    virtual void dequeueTcpLayerMsg(int msgLengthP);

    /**
     * Utility function: returns how many bytes are available in the queue.
     */
    ulong getBytesAvailable();

    /**
     * Called when the TCP wants to send or retransmit data, it constructs
     * a TCP segment which contains the data from the requested sequence
     * number range. The actually returned segment may contain less then
     * maxNumBytes bytes if the subclass wants to reproduce the original
     * segment boundaries when retransmitting.
     *
     * called from inside of send_callback()
     * called before called the send() to IP layer
     * @param tcpDataP: the tcp segment (with tcp header) created by LWIP
     * @param tcpLenthP: the length of tcp segment.
     */
    virtual TCPSegment * createSegmentWithBytes(const void* tcpDataP, int tcpLengthP);

    /**
     * Tells the queue that bytes up to (but NOT including) seqNum have been
     * transmitted and ACKed, so they can be removed from the queue.
     */
    virtual void discardAckedBytes();
};

class INET_API TcpLwipMsgBasedReceiveQueue : public TcpLwipReceiveQueue
{
  public:
    class SeqCmpLess
    {
      public:
        bool operator()(uint32 a, uint32 b) const { return seqLess(a, b); }
    };

  protected:
    typedef std::map<uint32, cPacket *, SeqCmpLess> PayloadList;
    PayloadList payloadListM;

  public:
    /**
     * Ctor.
     */
    TcpLwipMsgBasedReceiveQueue();

    /**
     * Virtual dtor.
     */
    virtual ~TcpLwipMsgBasedReceiveQueue();

    /**
     * Set the connection.
     */
    virtual void setConnection(TcpLwipConnection *connP);

    /**
     * Called when a TCP segment arrives, it should extract the payload
     * from the segment and store it in the receive queue. The segment
     * object should be deleted by queue.
     *
     * The method should return the number of bytes to copied to buffer.
     *
     * The method should fill the bufferP for data sending to LWIP stack
     *
     * called back from lwip::tcp_input()
     */
    virtual void insertBytesFromSegment(TCPSegment *tcpsegP, uint32 seqNo, void* bufferP, size_t bufferLengthP);

    /**
     * The method called when data received from LWIP
     * The method should set status of the data in queue to received
     * called after socket->read_data() successful
     */
    virtual void enqueueTcpLayerData(void* dataP, int dataLengthP);

    /**
     * Should create a packet to be passed up to the app, up to (but NOT
     * including) the given sequence no (usually rcv_nxt).
     * It should return NULL if there's no more data to be passed up --
     * this method is called several times until it returns NULL.
     *
     * called after socket->read_data() successful
     */
    virtual cPacket *extractBytesUpTo();

    /**
     * Returns the number of bytes (out-of-order-segments) currently buffered in queue.
     */
    virtual uint32 getAmountOfBufferedBytes();

    /**
     * Returns the number of blocks currently buffered in queue.
     */
    virtual uint32 getQueueLength();

    /**
     * Shows current queue status.
     */
    virtual void getQueueStatus();

    /**
     * notify the queue about output messages
     *
     * called when connM send out a packet.
     * for read AckNo, if have
     */
    virtual void notifyAboutSending(const TCPSegment *tcpsegP);

  protected:
    long int bytesInQueueM;
};

#endif
