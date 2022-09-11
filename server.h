#ifndef SERVER_HEADER
#define SERVER_HEADER

inline long clientSeq;
inline long serverSeq;
inline bool timedOut;
inline int soc;

void sendLS(string ls);

#endif