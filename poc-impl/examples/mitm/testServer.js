const https = require('https');
const fs = require('fs');
const proxy = require('http-proxy').createProxyServer({
    target: 'https://google.com'
});

proxy.on('proxyReq', function(proxyReq, req, res, options) {
//    console.log('Setting host')
//    proxyReq.setHeader('host', 'andrej-mohar.com');
    console.log(proxyReq.getHeaders());
});

// http.createServer({}, function (req, res, next) {
//     console.log("Request!");
//     //var fullUrl = req.protocol + '://' + req.get('host') + req.originalUrl;
// 	//console.log(req);
    
//     proxy.web(req, res, {
//         target: 'http://andrej-mohar.com'
//     }, next);

// }).listen(80);

https.createServer({
    key: fs.readFileSync('../certs/cert.key'),
    cert: fs.readFileSync('../certs/cert.crt'),
    ca: [
        fs.readFileSync('../certs/spoofed_ca.crt')
    ]
}, function (req, res, next) {
    console.log("Request!");

    proxy.web(req, res, {
        target: 'https://google.com'
    }, next);
})
.listen(443);


console.log("Created.");