# CVE-2017-5638

## Escopo correto
Este projeto é uma simulação em C com sockets TCP/HTTP, limitada a `127.0.0.1`. Representa o fluxo conceitual `Content-Type` → validação/processamento multipart simplificado → caminho de erro → resposta HTTP. Compara uma variante didática insegura, que reflete entrada não confiável no erro, com uma variante mitigada, que valida e retorna mensagem genérica.

**Limitação essencial:** não é Apache Struts, não implementa Jakarta Multipart nem OGNL, não reproduz a exploração original e não executa comandos. `[LAB_CONTROLLED_MARKER]` é texto inerte exclusivo do laboratório. Os resultados comprovam o comportamento desta simulação, não RCE real nem a presença da CVE no programa.

## Dependências (Ubuntu)
```bash
sudo apt update
sudo apt install build-essential make
```

## Compilar e testar
```bash
make clean
make all
make test
```

## Demonstração
Terminal 1:
```bash
./build/server
```
Terminal 2:
```bash
./build/client
./build/client --invalid
```
Encerre o servidor com Ctrl+C. Inicie a versão mitigada:
```bash
./build/server --patched
```
No segundo terminal, repita os dois comandos de cliente. Compare status HTTP, reflexão do marcador e logs.

## Avaliação de desempenho
Execute:
```bash
./build/benchmark
```
O benchmark mede o tempo médio da função de tratamento (microsegundos por requisição), com 200.000 iterações por cenário. Repita 5 vezes e calcule média/desvio padrão manualmente ou registre cada execução. Não confundir este microbenchmark com latência de rede ou desempenho do Apache Struts.

## Segurança do laboratório
- Bind exclusivo em 127.0.0.1.
- Não enviar requisições para hosts externos.
- Não há execução de comandos nem interpretação de expressões.
