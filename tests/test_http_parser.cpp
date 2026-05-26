#include "Protocols/Http/HttpParser.hpp"

#include <gtest/gtest.h>

using Protocols::Http::HttpParser;

TEST(HttpParser, ParsesSimpleGet) {
    HttpParser p;
    const char* req = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
    auto result     = p.feed(req, strlen(req));
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->method, "GET");
    EXPECT_EQ(result->uri, "/index.html");
    EXPECT_EQ(result->version, "HTTP/1.1");
    EXPECT_EQ(result->headers.at("Host"), "localhost");
}

TEST(HttpParser, ReturnsNulloptForPartialData) {
    HttpParser p;
    const char* partial = "GET /index.html HTTP/1.1\r\n";
    auto result         = p.feed(partial, strlen(partial));
    EXPECT_FALSE(result.has_value());
}

TEST(HttpParser, ResetClearsBuffer) {
    HttpParser p;
    const char* req = "GET / HTTP/1.1\r\n\r\n";
    p.feed(req, strlen(req));
    p.reset();
    auto result = p.feed(req, strlen(req));
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->uri, "/");
}
