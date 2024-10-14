This is a Test Ceritificate Authority, only to be used for testing.

#Root private key
openssl genrsa -out ca.key 2048

#Root certificate
openssl req -new -x509 -key ca.key -out ca.crt -days 10000 -batch

#Mid CSR and key
openssl req -new -out mid.csr -config mid.conf

#Sign mid.crt
openssl x509 -req -in mid.csr -extfile mid.ext -CA ca.crt -CAkey ca.key -CAcreateserial -out mid.crt -days 10000

#My CSR and key
openssl req -new -out my.csr -config my.conf
# View it with: openssl req -in my.csr -noout -text

#Sign my.crt
openssl x509 -req -in my.csr -CA mid.crt -CAkey mid.key -CAcreateserial -out my.crt -days 10000
