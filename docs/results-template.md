# Registro dos experimentos

Preencher apenas após executar localmente.

| Cenário | Modo | Status HTTP observado | Marcador refletido? | Tempo benchmark (us/req) |
|---|---|---:|---|---:|
| Multipart válido | Simulado vulnerável | | | |
| Multipart válido | Mitigado | | | |
| Content-Type inválido + marcador | Simulado vulnerável | | | |
| Content-Type inválido + marcador | Mitigado | | | |

## Interpretação
A evidência esperada da simulação é: entrada válida aceita nos dois modos; entrada inválida resulta em erro 500 com reflexão no modo simulado vulnerável; a versão mitigada rejeita com 400 e não reflete o marcador. Confirmar os valores observados antes de escrever conclusões.

O benchmark é microbenchmark da função local, não inclui sockets, rede, Apache Struts ou latência real. Repetir pelo menos cinco vezes e informar ambiente, compilador e dispersão.

## Limitações
Não afirmar execução remota de código. A simulação ilustra propagação de entrada não confiável no caminho de erro e uma estratégia defensiva.
