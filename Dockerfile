FROM debian:latest

WORKDIR /app

COPY ./out/Neptune-OS /app/Neptune-OS

COPY ./config /app/config

CMD [ "/app/Neptune-OS" ]