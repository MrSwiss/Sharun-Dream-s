#include "globale.h"

#ifdef WIN32
typedef struct pmtx_t {
	HANDLE	mtx;
	HANDLE	cnd;
} pmtx_t;
void *Mutex_Init()
{
	pmtx_t *pmtx = malloc(sizeof(pmtx_t));
	pmtx->mtx = CreateMutex(NULL, FALSE, NULL);
	if (!pmtx->mtx) {
		DEBUG("%s (%i) :: CreateMutex error (%d) !\n", __func__, __LINE__, GetLastError());
		free(pmtx);
		return NULL;
	}
	pmtx->cnd = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!pmtx->cnd) {
		DEBUG("%s (%i) :: CreateEvent error (%d) !\n", __func__, __LINE__, GetLastError());
		CloseHandle(pmtx->mtx);
		free(pmtx);
		return NULL;
	}
	return pmtx;
}
void Mutex_Close(void *arg)
{
	pmtx_t *pmtx = arg;
	CloseHandle(pmtx->mtx);
	CloseHandle(pmtx->cnd);
	free(pmtx);
}
void Mutex_Lock(void *arg)
{
	pmtx_t *pmtx = arg;
	WaitForSingleObject(pmtx->mtx, INFINITE);
}
void Mutex_Unlock(void *arg)
{
	pmtx_t *pmtx = arg;
	ReleaseMutex(pmtx->mtx);
}
void Mutex_Pause(void *arg)
{
	pmtx_t *pmtx = arg;
	ResetEvent(pmtx->cnd);
	Mutex_Unlock(pmtx);
}
void Mutex_Resume(void *arg)
{
	pmtx_t *pmtx = arg;
	Mutex_Lock(pmtx);
	SetEvent(pmtx->cnd);
	Mutex_Unlock(pmtx);
}
#else
typedef struct pmtx_t {
	pthread_mutex_t	mtx;
	pthread_cond_t	cnd;
} pmtx_t;

void *Mutex_Init()
{
	pmtx_t *pmtx = malloc(sizeof(pmtx_t));
	int ret = pthread_mutex_init(&pmtx->mtx, NULL);
	if (ret) {
		DEBUG("%s (%i) :: CreateMutex error (%d) !\n", __func__, __LINE__, ret);
		return NULL;
	}
	return pmtx;
}
void Mutex_Close(void *arg)
{
	pmtx_t *pmtx = arg;
	pthread_mutex_destroy(&pmtx->mtx);
	free(pmtx);
}
void Mutex_Lock(void *arg)
{
	pmtx_t *pmtx = arg;
	pthread_mutex_lock(&pmtx->mtx);
}
void Mutex_Unlock(void *arg)
{
	pmtx_t *pmtx = arg;
	pthread_mutex_unlock(&pmtx->mtx);
}
void Mutex_Pause(void *arg)
{
	pmtx_t *pmtx = arg;
	pthread_cond_wait(&pmtx->cnd, &pmtx->mtx);
	Mutex_Unlock(pmtx);
}
void Mutex_Resume(void *arg)
{
	pmtx_t *pmtx = arg;
	Mutex_Lock(pmtx);
	pthread_cond_signal(&pmtx->cnd);
	Mutex_Unlock(pmtx);
}
#endif
void Mutex_SPause(void *arg)
{
	Mutex_Lock(arg);
	Mutex_Lock(arg);
	Mutex_Unlock(arg);
}

