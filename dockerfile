# Free The Whales, Shut Down Sea World

FROM python:2.7
LABEL maintainer="CuteLab <https://www.cutelab.nyc>"

COPY tag /tag
RUN python -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/$(cat /tag)/scripts/get-platformio.py)"
ENV PATH="/root/.platformio/penv/bin:${PATH}"

RUN rm /tag
RUN platformio platform update

WORKDIR /project

COPY . ./project

ENTRYPOINT ["pio", "run"]