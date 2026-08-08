#include "Pages.h"

/**
 * @brief Page used when the dashboard cannot be loaded from the storage device.
 *
 * This page is displayed when the application cannot find the dashboard
 * files on the storage device. It provides instructions to the user to
 * verify that the storage device is mounted and that the required files
 * exist.
 *
 */
const char* Pages::FALLBACK = R"rawliteral(
<!DOCTYPE html>

<html>

<head>

<meta charset="UTF-8">

<title>Ch3rryB0mb Recovery</title>

<style>

body
{
    background:#141414;
    color:#E0E0E0;
    font-family:Arial, Helvetica, sans-serif;
    text-align:center;
    padding-top:80px;
}

h1
{
    color:#ff2d55;
}

.card
{
    width:500px;
    margin:auto;
    padding:25px;

    background:#222;

    border-radius:12px;

    box-shadow:0px 0px 25px rgba(255,45,85,.25);
}

</style>

</head>

<body>

<div class="card">

<h1>🍒 Ch3rryB0mb</h1>

<h2>Recovery Mode</h2>

<p>
The dashboard could not be loaded.
</p>

<p>
Please verify that the storage device is mounted and
that <b>/web/index.html</b> exists.
</p>

</div>

</body>

</html>
)rawliteral";

/**
 * @brief Error page used when a requested resource cannot be found.
 *
 * This page is displayed when the user requests a resource that does not
 * exist on the web server. It provides a simple 404 error message.
 *
 */
const char* Pages::ERROR404 = R"rawliteral(
<!DOCTYPE html>

<html>

<head>

<title>404</title>

</head>

<body>

<h1>404</h1>

<p>The requested page could not be found.</p>

</body>

</html>
)rawliteral";