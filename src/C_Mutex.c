#include "globale.h"

#ifdef WIN32
void *Mutex_Init()
{
	void *mtx = CreateMutex(NULL, FALSE, NULL);
	if (!mtx) {
		DEBUG("%s (%i) :: CreateMutex error (%d) !\n", __func__, __LINE__, GetLastError());
		return 1;
	}
	return ntx;
}
void Mutex_Close(void *mtx)
{
	CloseHandle(mtx);
}
void Mutex_Lock(void *mtx)
{
	WaitForSingleObject(mtx, INFINITE);
}
void Mutex_Unlock(void *mtx)
{
	ReleaseMutex(mtx);
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
	pmtx_t *pmtx = (pmtx_t *)arg;
	pthread_mutex_destroy(&pmtx->mtx);
	free(pmtx);
}
void Mutex_Lock(void *arg)
{
	pmtx_t *pmtx = (pmtx_t *)arg;
	pthread_mutex_lock(&pmtx->mtx);
}
void Mutex_Unlock(void *arg)
{
	pmtx_t *pmtx = (pmtx_t *)arg;
	pthread_mutex_unlock(&pmtx->mtx);
}
void Mutex_SPause(void *arg)
{
	Mutex_Lock(arg);
	Mutex_Lock(arg);
	Mutex_Unlock(arg);
}
void Mutex_Pause(void *arg)
{
	pmtx_t *pmtx = (pmtx_t *)arg;
	pthread_cond_wait(&pmtx->cnd, &pmtx->mtx);
	Mutex_Unlock(arg);
}
void Mutex_Resume(void *arg)
{
	pmtx_t *pmtx = (pmtx_t *)arg;
	Mutex_Lock(arg);
	pthread_cond_signal(&pmtx->cnd);
	Mutex_Unlock(arg);
}
#endif
