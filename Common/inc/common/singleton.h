#pragma once

#define DECLARE_SINGLETON_CLASS(TClass) \
    public: \
        static TClass Instance; \
        static TClass* GetInstance() { return &TClass::Instance; } \
        void Release();\
    protected: \
        TClass(); \
        virtual ~TClass(); \
    private: \
        TClass(const TClass& _instance) = default; \
        TClass& operator=(const TClass& _instance) { return *this; }

#define DEFINE_SINGLETON_CLASS(TClass, ConstructorBody, DestructorBody, ReleaseBody) \
        TClass TClass::Instance; \
        TClass::TClass() \
            ConstructorBody \
        TClass::~TClass() \
            DestructorBody\
        void TClass::Release()\
            ReleaseBody