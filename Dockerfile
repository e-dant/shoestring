FROM httpd:alpine

ARG PORT
ENV PORT=${PORT:-8080}

RUN apk add --no-cache bash

COPY ./www/out/ /usr/local/apache2/htdocs/
COPY ./httpd/httpd.conf /usr/local/apache2/conf/httpd.conf

EXPOSE $PORT
