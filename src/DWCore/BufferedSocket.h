/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: BufferedSocket.h
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

//temporary settings
const int AuthSocketPort = 3724;

#ifndef _BUFFEREDSOCKET_H_
#define _BUFFEREDSOCKET_H_

#include <ace/Basic_Types.h>
#include <ace/Synch_Traits.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Message_Block.h>
#include <ace/Basic_Types.h>

#include <string>

class BufferedSocket: public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
    protected:
        typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> Base;

        virtual void OnRead(void) { }
        virtual void OnAccept(void) { }
        virtual void OnClose(void) { }

    public:
        BufferedSocket(void);
        virtual ~BufferedSocket(void);

        size_t recv_len(void) const;
        bool recv_soft(char *buf, size_t len);
        bool recv(char *buf, size_t len);
        void recv_skip(size_t len);

        bool send(const char *buf, size_t len);

        const std::string& get_remote_address(void) const;

        virtual int open(void *);

        void close_connection(void);

        virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
        virtual int handle_output(ACE_HANDLE = ACE_INVALID_HANDLE);

        virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE,
                ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

    private:
        ssize_t noblk_send(ACE_Message_Block &message_block);

    private:
        ACE_Message_Block input_buffer_;

    protected:
        std::string remote_address_;

};

#endif /* _BUFFEREDSOCKET_H_ */
