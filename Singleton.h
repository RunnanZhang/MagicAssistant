#ifndef HISIGNCORE_SINGLETON_H
#define HISIGNCORE_SINGLETON_H

#include <QMutex>
#include <QScopedPointer>

template <typename T>
class Singleton
{
public:
    static T* getInstance();

private:
    Singleton(const Singleton&);
    Singleton<T>& operator = (const Singleton&);

    static QMutex s_mutex;
    static QScopedPointer<T> s_instance;
};

template <typename T> QMutex Singleton<T>::s_mutex;
template <typename T> QScopedPointer<T> Singleton<T>::s_instance;

template <typename T>
T* Singleton<T>::getInstance()
{
    if(s_instance.isNull()) {
        s_mutex.lock();

        if(s_instance.isNull()) {
            s_instance.reset(new T());
        }

        s_mutex.unlock();
    }

    return s_instance.data();
}


#define SINGLETON(Class)                        \
public:                                         \
    static Class* getInstance() {               \
        return Singleton<Class>::getInstance(); \
    }                                           \
private:                                        \
    Class();                                    \
    ~Class();                                   \
    Class(const Class &other);                  \
    Class& operator=(const Class &other);       \
    friend class Singleton<Class>;              \
    friend struct QScopedPointerDeleter<Class>;


#endif // HISIGNCORE_SINGLETON_H
