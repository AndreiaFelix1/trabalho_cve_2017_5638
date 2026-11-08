# Checklist de demonstração
1. Mostrar dependências e compilação.
2. Executar `make test` e registrar os oito testes.
3. Iniciar `./build/server` e testar cliente válido e inválido.
4. Capturar HTTP 200 para válido e HTTP 500/reflexão para inválido.
5. Reiniciar com `./build/server --patched`.
6. Repetir e capturar HTTP 200 para válido e HTTP 400 sem reflexão para inválido.
7. Executar `./build/benchmark` cinco vezes e guardar os resultados.
8. Explicar que é simulação e não exploração real da RCE.
