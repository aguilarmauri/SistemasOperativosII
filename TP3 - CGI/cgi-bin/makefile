all: a_info_recursos.c b_get_telemetry.c b_get_datta.c b_erase_datta.c c_modulos.c c_ins_modulo.c c_qui_modulo.c
        gcc -o a_info_recursos.cgi a_info_recursos.c -lm
        gcc -o b_get_telemetry.cgi b_get_telemetry.c
        gcc -o b_get_datta.cgi b_get_datta.c
        gcc -o b_erase_datta.cgi b_erase_datta.c
        gcc -o c_modulos.cgi c_modulos.c -lm
        gcc -o c_ins_modulo.cgi c_ins_modulo.c -lm
        gcc -o c_qui_modulo.cgi c_qui_modulo.c -lm
        gcc -o b_simulador.cgi b_simulador.c
        cd /var/www/cgi-bin && chmod +x *.cgi

clean:
      	rm a_info_recursos.cgi
        rm b_get_telemetry.cgi
        rm b_get_datta.cgi
        rm b_erase_datta.cgi
        rm c_modulos.cgi
        rm c_ins_modulo.cgi
        rm c_qui_modulo.cgi
