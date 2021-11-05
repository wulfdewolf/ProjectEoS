float bark_to_hertz(float bark) {
    float new_bark;
    if(bark < 2.0) {
        new_bark = (bark-0.3) / 0.85;
    } else if(bark > 20.1) {
        new_bark = (bark+4.422) / 1.22;
    } else {
        new_bark = bark;
    }
    return 1960.0 * ((new_bark + 0.53) / (26.28 - new_bark));
}

float hertz_to_bark(float hertz) {
    float bark = ((26.81 * hertz) / (1960.0 + hertz)) - 0.53;
    if(bark < 2) {
        return bark + 0.15 * (2.0 - bark);
    } else if(bark > 20.1) {
        return bark + 0.22 * (bark - 20.1);
    } else {
        return bark;
    }
}