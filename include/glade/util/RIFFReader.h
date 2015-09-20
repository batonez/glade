#pragma once

// FIXME get rid of windows.h here
#include <windows.h>

class RIFFReader
{
  public:
    static HRESULT FindChunk(HANDLE hFile, unsigned long fourcc, unsigned long & dwChunkSize, unsigned long & dwChunkDataPosition)
    {
        HRESULT hr = S_OK;
        if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
            return HRESULT_FROM_WIN32( GetLastError() );

        unsigned long chunkType;
        unsigned long chunkDataSize;
        unsigned long riffDataSize = 0;
        unsigned long fileType;
        unsigned long bytesRead = 0;
        unsigned long offset = 0;

        while (hr == S_OK)
        {
            unsigned long howManyWasRead;
            
            if( 0 == ReadFile( hFile, &chunkType, sizeof(unsigned long), &howManyWasRead, NULL ) )
                hr = HRESULT_FROM_WIN32( GetLastError() );

            if( 0 == ReadFile( hFile, &chunkDataSize, sizeof(unsigned long), &howManyWasRead, NULL ) )
                hr = HRESULT_FROM_WIN32( GetLastError() );

            switch (chunkType)
            {
            case fourccRIFF:
                riffDataSize = chunkDataSize;
                chunkDataSize = 4;
                if( 0 == ReadFile( hFile, &fileType, sizeof(unsigned long), &howManyWasRead, NULL ) )
                    hr = HRESULT_FROM_WIN32( GetLastError() );
                break;

            default:
                if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, chunkDataSize, NULL, FILE_CURRENT ) )
                return HRESULT_FROM_WIN32( GetLastError() );            
            }

            offset += sizeof(unsigned long) * 2;
            
            if (chunkType == fourcc)
            {
                dwChunkSize = chunkDataSize;
                dwChunkDataPosition = offset;
                return S_OK;
            }

            offset += chunkDataSize;
            
            if (bytesRead >= riffDataSize) return S_FALSE;

        }

        return S_OK;
        
    }

    static HRESULT ReadChunkData(HANDLE hFile, void * buffer, unsigned long buffersize, unsigned long bufferoffset)
    {
        HRESULT hr = S_OK;
        if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferoffset, NULL, FILE_BEGIN ) )
            return HRESULT_FROM_WIN32( GetLastError() );
        unsigned long howManyWasRead;
        if( 0 == ReadFile( hFile, buffer, buffersize, &howManyWasRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );
        return hr;
    }
};