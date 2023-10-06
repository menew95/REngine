#pragma once

#include <common/types.h>

#include <Windows.h>
#include <vector>

class StringHelper
{
public:
    /**
     * @brief ���ڿ��� �������մϴ�.
     * @param _format �������� ���ڿ� �����Դϴ�.
     * @param ..._args (optional) �������� ���ڿ��� �� �����Դϴ�.
     * @return
    */
    template <typename ... Args>
    static tstring Format(const tstring& _format, Args ... _args);

    /**
     * @brief ���ڿ��� �������մϴ�.
     * @param _format �������� ���ڿ� �����Դϴ�.
     * @param ..._args (optional) �������� ���ڿ��� �� �����Դϴ�.
     * @return �����õ� ���ڿ��Դϴ�.
    */
    template <typename ... Args>
    static tstring Format(const TCHAR* _format, Args ... _args);

    /**
     * @brief ���ڿ��� �־��� ��ū �������� �����ϴ�.
     * @param _str ��ū���� ���� ���ڿ��Դϴ�.
     * @param _token ���� ������ ��ū�Դϴ�.
     * @return ��ū���� �������� ���ڿ� �迭�Դϴ�.
    */
    static std::vector<tstring> Split(const tstring& _str, const tstring& _token);

    /**
     * @brief string�� wstring���� ��ȯ�մϴ�.
    */
    static std::wstring StringToWString(const char* _string);

    /**
     * @brief string�� wstring���� ��ȯ�մϴ�.
    */
    static std::wstring StringToWString(const std::string& _string);

    /**
     * @brief wstring�� string���� ��ȯ�մϴ�.
    */
    static std::string WStringToString(const wchar_t* _string);

    /**
     * @brief wstring�� string���� ��ȯ�մϴ�.
    */
    static std::string WStringToString(const std::wstring& _string);

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� wstring���� ��ȯ�մϴ�.
    */
    static std::wstring ToWString(const std::string& _string)
    {
        return StringToWString(_string);
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� wstring���� ��ȯ�մϴ�.
    */
    static std::wstring ToWString(const std::wstring& _string)
    {
        return _string;
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� wstring���� ��ȯ�մϴ�.
    */
    static std::wstring ToWString(const char* _string)
    {
        return StringToWString(_string);
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� wstring���� ��ȯ�մϴ�.
    */
    static std::wstring ToWString(const wchar_t* _string)
    {
        return std::wstring(_string);
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� string���� ��ȯ�մϴ�.
    */
    static std::string ToString(const std::wstring& _string)
    {
        return WStringToString(_string);
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� string���� ��ȯ�մϴ�.
    */
    static std::string ToString(const std::string& _string)
    {
        return _string;
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� string���� ��ȯ�մϴ�.
    */
    static std::string ToString(const wchar_t* _string)
    {
        return WStringToString(_string);
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� string���� ��ȯ�մϴ�.
    */
    static std::string ToString(const char* _string)
    {
        return std::string{ _string };
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� tstring���� ��ȯ�մϴ�.
    */
    static tstring ToTString(const std::string& _string)
    {
#if defined (_UNICODE)
        return StringToWString(_string);
#else
        return _string;
#endif
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� tstring���� ��ȯ�մϴ�.
    */
    static tstring ToTString(const std::wstring& _string)
    {
#if defined (_UNICODE)
        return _string;
#else
        return WStringToString(_string);
#endif
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� tstring���� ��ȯ�մϴ�.
    */
    static tstring ToTString(const char* _string)
    {
#if defined( _UNICODE )
        return StringToWString(_string);
#else
        return std::string{ _string };
#endif
    }

    /**
     * @brief � ���ڿ� Ÿ���̵� ��� ���� tstring���� ��ȯ�մϴ�.
    */
    static tstring ToTString(const wchar_t* _string)
    {
#if defined( _UNICODE )
        return std::wstring{ _string };
#else
        return WStringToString(_string);
#endif
    }
};

#include "common\string_helper.inl"