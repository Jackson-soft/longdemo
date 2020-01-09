#include <cstdlib>
#include <cstring>
#include <fmt/format.h>
#include <iostream>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <string_view>

// 编码
std::string Encode(const std::string_view input)
{
    ::BUF_MEM *bptr{nullptr};
    auto b64 = ::BIO_new(BIO_f_base64());
    auto bio = ::BIO_new(BIO_s_mem());
    bio      = ::BIO_push(b64, bio);
    ::BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    ::BIO_write(bio, input.data(), input.size());
    BIO_flush(bio);
    ::BIO_get_mem_ptr(b64, &bptr);

    char *buffer = new char[bptr->length];
    std::memcpy(buffer, bptr->data, bptr->length);
    ::BIO_free_all(bio);
    std::string ret{buffer};
    delete[] buffer;
    return ret;
}

// 解码
std::string Decode(const std::string_view input)
{
    auto b64 = ::BIO_new(::BIO_f_base64());
    ::BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    auto bio     = ::BIO_new_mem_buf(input.data(), input.size());
    bio          = ::BIO_push(b64, bio);
    char *outPut = new char[input.size()];
    ::BIO_read(bio, outPut, input.size());

    ::BIO_free_all(bio);
    std::string ret{outPut};
    delete[] outPut;
    return ret;
}

int main()
{
    std::cout << Encode("sayHello") << std::endl;
    std::cout << Decode("c2F5SGVsbG8=") << std::endl;
    std::string myKey = fmt::format("{}{}", "KpNUimOEaQyRNF4OBribaN6rjoSIPx", "&");

    std::string myData{"hello world"};

    auto r = ::HMAC(
        EVP_sha1(), myKey.data(), myKey.length(), (unsigned char *)myData.c_str(), myData.length(), nullptr, nullptr);
    std::string src{(char *)r};
    std::cout << Encode(src) << std::endl;
    return EXIT_SUCCESS;
}
