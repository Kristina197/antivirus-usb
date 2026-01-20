FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Устанавливаем русскую локаль
RUN apt-get update && apt-get install -y locales && \
    locale-gen ru_RU.UTF-8 && \
    update-locale LANG=ru_RU.UTF-8

ENV LANG=ru_RU.UTF-8
ENV LANGUAGE=ru_RU:ru
ENV LC_ALL=ru_RU.UTF-8

RUN apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    qt6-base-dev \
    qt6-l10n-tools \
    libqt6sql6-sqlite \
    libsqlite3-dev \
    libudev-dev \
    libssl-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libegl1-mesa-dev \
    libglx-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-render-util0 \
    libxcb-xinerama0 \
    libxcb-xfixes0 \
    libxcb-shape0 \
    libxcb-xkb1 \
    libxcb-cursor0 \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN mkdir -p build && cd build && \
    cmake .. && \
    make -j$(nproc)

RUN mkdir -p /root/.local/share/antivirus-usb && \
    if [ -f data/antivirus.db ]; then cp data/antivirus.db /root/.local/share/antivirus-usb/; \
    elif [ -f antivirus.db ]; then cp antivirus.db /root/.local/share/antivirus-usb/; \
    else echo "Database will be created on first run"; fi

WORKDIR /app/build

# Используем стандартную светлую тему Qt Fusion
ENV QT_STYLE_OVERRIDE=Fusion

CMD ["./AntivirusUSB"]
