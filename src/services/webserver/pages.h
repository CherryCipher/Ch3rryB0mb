#pragma once

/**
 * @namespace Pages
 * @brief Contains embedded HTML pages used by the web server.
 *
 * These pages are compiled into the firmware and are only used when
 * the requested content cannot be served from the storage device.
 *
 * Typical uses include:
 * - Fallback dashboard
 * - HTTP 404 page
 * - Maintenance page
 * - Recovery page
 */
namespace Pages
{
    /**
    * @brief Page used when the dashboard cannot be loaded from the storage device.
    */
    extern const char* FALLBACK;

    /**
    * @brief Error page used when a requested resource cannot be found.
    */
    extern const char* ERROR404;
}