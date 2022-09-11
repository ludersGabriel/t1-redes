#ifndef SERVER_HEADER
#define SERVER_HEADER

inline long clientSeq;
inline long serverSeq;
inline bool timedOut;
inline int soc;

void sendStream(FILE* stream, int type);

void sendEnd();

#endif