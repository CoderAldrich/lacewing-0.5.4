# lacewing-0.5.4
编译好可以直接用的lacewing，在原版的基础上增加了编译成完整静态库的属性，不再依赖dll

使用时，头部增加以下内容
```
#include <windows.h>，
#include <lacewing.h>
#include <string>
#include <shlwapi.h>
#include <string>
#include <fstream> 
#include <iosfwd>

using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")
#pragma comment(lib,"Secur32.lib")
#pragma comment(lib,"crypt32.lib")
#pragma comment(lib,"shlwapi.lib")
```

同时需要在预处理器中增加宏定义`_lacewing_static`

---------------------------------------------------------------------------------------------------------------------------------

# [lacewing简介](http://udp.github.io/lacewing/docs/)
liblacewing是一个跨平台的，为C/C++提供的高级网络开源库，旨在提供一个简明的类，做到能够扩展并且可以平台优化（支持IOCP/EPOLL/KQUEUE等）。

## Classes
### [lacewing::eventpump](http://udp.github.io/lacewing/docs/eventpump/)

**eventpump** is a default implementation of pump provided by the library, powered by IOCP, kqueue or epoll.

As there is no single model for event handling that would suit every application, eventpump provides several different modes of operation:

- [tick](http://udp.github.io/lacewing/docs/eventpump/tick.html) polls aggressively, suited to a game loop;

- [start_eventloop](http://udp.github.io/lacewing/docs/eventpump/start_eventloop.html) starts a simple "block forever" event loop, suited to console applications and daemons;

- [start_sleepy_ticking](http://udp.github.io/lacewing/docs/eventpump/start_sleepy_ticking.html) enables a threaded callback to request the application call tick as soon as possible, suited to applications with an existing message pump (such as Win32 GUI applications)

### [lacewing::webserver](http://udp.github.io/lacewing/docs/webserver/)
**webserver** aims to provide an embeddable, flexible HTTP server using liblacewing classes as a base.

Ease of embedding makes **webserver** ideal for things such as remote administration interfaces, and through the use of [eventpump](http://udp.github.io/lacewing/docs/eventpump/), **webserver** is able to scale well and adapt transparently to different ways of handling I/O.

Features of webserver include:

- Simple hooks for requests, with easy access to GET and POST data
- Support for cookies and sessions
- Support for HTTPS (see host_secure)
- HTTP file upload

>**webserver** uses [server](http://udp.github.io/lacewing/docs/server/) as a base for socket communication.

#### on_get (webserver)
```cpp
// C++ style
void on_get (lacewing::webserver webserver, lacewing::webserver_request request)
{
/*on_get的响应*/
}

webserver->on_get (on_get); /* to register the hook */
```

```cpp
// C style
void on_get (lw_ws webserver, lw_ws_request)
{
/*on_get的响应*/
}

lw_ws_on_get (webserver, on_get); /* to register the hook */
```
**Description**
The on_get hook is triggered for an incoming HTTP `GET` request.

This hook marks the beginning of the lifetime of a [request](http://udp.github.io/lacewing/docs/webserver/request) object. If [enable_manual_finish](http://udp.github.io/lacewing/docs/webserver/enable_manual_finish.html) has been called, the [request](http://udp.github.io/lacewing/docs/webserver/request) object will be valid until either [request::finish](http://udp.github.io/lacewing/docs/webserver/request/finish.html) is called OR the [on_disconnect](http://udp.github.io/lacewing/docs/webserver/on_disconnect.html) hook triggers. Otherwise, the [request](http://udp.github.io/lacewing/docs/webserver/request) object is only valid until the hook returns.

The application may configure a response in this hook with functions such as [request::header](http://udp.github.io/lacewing/docs/webserver/request/header.html), and request::cookie, and write the response data with [stream::write](http://udp.github.io/lacewing/docs/stream/write.html) and [stream::write_file](http://udp.github.io/lacewing/docs/stream/write_file.html).

Nothing will actually be written to the socket while the request object is still valid. This ensures that an accurate `Content-Length` header can be generated, and that headers can be changed throughout the lifetime of the [request](http://udp.github.io/lacewing/docs/webserver/request).

#### on_post (webserver)
```cpp
// C++ style
void on_get (lacewing::webserver webserver, lacewing::webserver_request &request)
{
}

webserver->on_post (on_post); /* to register the hook */
```
```cpp
// C style
void on_post (lw_ws webserver, lw_ws_request)
{
}

lw_ws_on_post (webserver, on_post); /* to register the hook */
```

**Description**
The *on_post* hook is triggered for an incoming HTTP `POST` request.

This hook marks the beginning of the lifetime of a [request](http://udp.github.io/lacewing/docs/webserver/request) object. If [enable_manual_finish](http://udp.github.io/lacewing/docs/webserver/enable_manual_finish.html) has been called, the [request](http://udp.github.io/lacewing/docs/webserver/request) object will be valid until either [request::finish](http://udp.github.io/lacewing/docs/webserver/request/finish.html) is called OR the [on_disconnect](http://udp.github.io/lacewing/docs/webserver/on_disconnect.html) hook triggers. Otherwise, the [request](http://udp.github.io/lacewing/docs/webserver/request) object is only valid until the hook returns.

The application may configure a response in this hook with functions such as [request::header](http://udp.github.io/lacewing/docs/webserver/request/header.html), and [request::cookie](http://udp.github.io/lacewing/docs/webserver/request/cookie.html), and write the response data with [stream::write](http://udp.github.io/lacewing/docs/stream/write.html) and [stream::write_file](http://udp.github.io/lacewing/docs/stream/write_file.html).

Nothing will actually be written to the socket while the [request](http://udp.github.io/lacewing/docs/webserver/request) object is still valid. This ensures that an accurate `Content-Length`header can be generated, and that headers can be changed throughout the lifetime of the [request](http://udp.github.io/lacewing/docs/webserver/request).

## Methods

### file_exists
```cpp
// C++ style
lw_bool file_exists (const char * filename)
```
```cpp
// C style
lw_bool lw_file_exists (const char * filename)
```
**Description**
Returns `true` if the file specified by `filename` exists, or `false` otherwise.

This function will also return `false` if the file specified by `filename` is a directory. To determine if a directory exists, use [path_exists](http://udp.github.io/lacewing/docs/path_exists.html) instead.

**Parameters**
*filename*
Any absolute or relative filename.

### path_exists
```cpp
// C++ style
lw_bool lacewing::path_exists (const char * filename)
```
```cpp
// C style
lw_bool lw_path_exists (const char * filename)
```
**Description**
Returns true if the directory specified by `filename` exists, or `false` otherwise.

This function will also return `false` if the path specified by `filename` is a file. To determine if a file exists, use [file_exists](http://udp.github.io/lacewing/docs/file_exists.html) instead.

**Parameters**
*filename*
Any absolute or relative filename.

### file_size
```cpp
// C++ style
size_t lacewing::file_size (const char * filename)
```
```cpp
// C style
size_t lw_file_size (const char * filename)
```
**Description**
Returns the size of a file in bytes, or `-1` if the file does not exist.

**Parameters**
*filename*
Any absolute or relative filename.

### md5
```cpp
// C++ style
void lacewing::md5 (char * output, const char * input, size_t length)
```
```cpp
// C style
void lw_md5 (char * output, const char * input, size_t length)
```
**Description**
Returns the MD5 hash of a block of data as the raw hash bytes.

The library itself does not include an MD5 implementation - this function is implemented through either OpenSSL or the [Windows Cryptography API](http://msdn.microsoft.com/en-us/library/aa380255%28v=vs.85%29.aspx).

**Parameters**
*output*
A buffer of at least 16 bytes to write the hash to.

*input*
A pointer to the input data.

*length* [optional]
The length of the input data, or -1 if the data is a null-terminated string.

### md5_hex
```cpp
// C++ style
void lacewing::md5_hex (char * output, const char * input, size_t length)
```
```cpp
// C style
void lw_md5_hex (char * output, const char * input, size_t length)
```
**Description**
Returns the MD5 hash of a block of data as a hexadecimal digest.

The library itself does not include an MD5 implementation - this function is implemented through either OpenSSL or the [Windows Cryptography API](http://msdn.microsoft.com/en-us/library/aa380255%28v=vs.85%29.aspx).

**Parameters**
*output*
A buffer of at least 33 bytes to write the hash digest to.

*input*
A pointer to the input data.

*length* [optional]
The length of the input data, or `-1` if the data is a null-terminated string.

### random
```cpp
// C++ style
lw_bool lacewing::random (char * buffer, size_t size)
```
```cpp
// C style
lw_bool lw_random (char * buffer, size_t size)
```
**Description**
Generates `size` bytes of random data, returning `true` on success or `false` on failure.

**Parameters**
*buffer*
A buffer of at least `size` bytes to store the generated random data.

*size*
The number of bytes of random data to generate.

### sha1
```cpp
// C++ style
void lacewing::sha1 (char * output, const char * input, size_t length)
```
```cpp
// C style
void lw_sha1 (char * output, const char * input, size_t length)
```
**Description**
Returns the SHA1 hash of a block of data as the raw hash bytes.

The library itself does not include an SHA1 implementation - this function is implemented through either OpenSSL or the [Windows Cryptography API](http://msdn.microsoft.com/en-us/library/aa380255%28v=vs.85%29.aspx).

**Parameters**
*output*
A buffer of at least 20 bytes to write the hash to.

*input*
A pointer to the input data.

*length* [optional]
The length of the input data, or `-1` if the data is a null-terminated string.

### sha1_hex
```cpp
// C++ style
void lacewing::sha1 (char * output, const char * input, size_t length)
```
```cpp
// C style
void lw_sha1 (char * output, const char * input, size_t length)
```
**Description**
Returns the SHA1 hash of a block of data as a hexadecimal digest.

The library itself does not include an SHA1 implementation - this function is implemented through either OpenSSL or the [Windows Cryptography API](http://msdn.microsoft.com/en-us/library/aa380255%28v=vs.85%29.aspx).

**Parameters**
*output*
A buffer of at least 41 bytes to write the hash digest to.

*input*
A pointer to the input data.

*length* [optional]
The length of the input data, or `-1` if the data is a null-terminated string.

### temp_path
```cpp
// C++ style
void lacewing::temp_path (char * buffer)
```
```cpp
// C style
void lw_temp_path (char * buffer)
```
**Description**
Determines the correct place to store temporary files on the current system.

The path retrieved will have a trailing slash.

**Parameters**
*buffer*
A buffer of at least 512 bytes to write the path to.

## Examples
### simple webserver
Outputs `"Hello world"` and [lacewing::version](http://udp.github.io/lacewing/docs/version.html) in response to any request
```cpp
#include <lacewing.h>

void on_get (lw_ws webserver, lw_ws_req req)
{
   lw_stream_writef (req, "Hello world from %s", lw_version ());
}

int main (int argc, char * argv[])
{
   lw_pump pump = lw_eventpump_new ();
   lw_ws webserver = lw_ws_new (pump);

   lw_ws_on_get (webserver, on_get);
   lw_ws_host (webserver, 8081);
    
   lw_eventpump_start_eventloop (pump);
    
   lw_ws_delete (webserver);
   lw_pump_delete (pump);

   return 0;
}
```

### ajax
即“Asynchronous Javascript And XML”(异步 JavaScript 和 XML),Simple long-poll AJAX example using jQuery
```cpp

#include <lacewing.h>

#include <string.h>
#include <iostream>
#include <list>

using namespace std;

list <lacewing::webserver_request> waiting;

void on_get (lacewing::webserver, lacewing::webserver_request request)
{
    request->write_file ("ajax.html");
    request->finish ();        
}

void on_post (lacewing::webserver, lacewing::webserver_request request)
{
    if (!strcmp (request->url (), "poll"))
    {
        waiting.push_back (request);
        return;
    }
    
    if (!strcmp (request->url (), "message"))
    {
        const char * message = request->POST ("message");
        
        request->writef ("Message from %s: %s\n",
                         request->address ()->tostring (),
                         message);

        /* Complete all waiting requests with the message */
        
        for(list <lacewing::webserver_request>::iterator it
                = waiting.begin (); it != waiting.end (); ++ it)
        {
            lacewing::webserver_request waiting_req = *it;
            
            waiting_req->write (message);
            waiting_req->finish ();
        }

        waiting.clear ();
        request->finish ();
        
        return;
    }
}

void on_disconnect (lacewing::webserver, lacewing::webserver_request request)
{
    for (list <lacewing::webserver_request>::iterator it
            = waiting.begin(); it != waiting.end (); ++ it)
    {
        if(*it == request)
        {
            waiting.erase (it);
            break;
        }
    }
}

int main (int argc, char * argv[])
{
    lacewing::eventpump eventpump = lacewing::eventpump_new ();
    lacewing::webserver webserver = lacewing::webserver_new (eventpump);

    /* Enabling this means we will have to call request->finish() to complete
     * a request.  Until request->finish() is called, requests will just hang,
     * which is exactly what we want for long-poll AJAX.
     */
       
    webserver->enable_manual_finish ();
    
    webserver->on_get (on_get);
    webserver->on_post (on_post);
    webserver->on_disconnect (on_disconnect);
   
    webserver->host (8080);    
    
    eventpump->start_eventloop();
    
    lacewing::webserver_delete (webserver);
    lacewing::pump_delete (eventpump);

    return 0;
}
```

```htmlbars

<html>
    <head>
        <title>liblacewing AJAX example</title>

        <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js" type="text/javascript"></script>
        <script type="text/javascript">

            $(document).ready(function()
            {
                $('#broadcast').click(function()
                {
                    var message = $('#message').val();
                    $('#message').val('');
                    
                    $.post('message', { message: message });
                });
                
                (function doPoll()
                {
                    $.post('poll', function(message)
                    {
                        if(!message)
                            return;
                            
                        $('#messages').prepend($('<li></li>').text(message));
                    
                        doPoll();
                    });
                    
                })();
            });
            
        </script>

    </head>
    <body>

        <label for="message">Type something to broadcast to everyone</label>
        <input type="text" id="message" />
        <button id="broadcast">Broadcast!</button>
        
        <br />
        Messages:
        
        <p><ul id="messages">
        
            
        </ul></p>
        
    </body>
</html>

```