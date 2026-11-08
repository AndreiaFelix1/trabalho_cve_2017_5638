# Arquitetura e protocolo

Cliente C --TCP loopback--> servidor C
Servidor: parser de linha HTTP/cabeçalhos → validador multipart reduzido → lógica de erro simulada → resposta HTTP.

A aplicação processa uma requisição por conexão. O cliente envia método POST, caminho `/upload`, cabeçalho Host, Content-Type e Content-Length. A demonstração compara um Content-Type multipart válido e um Content-Type inválido contendo um marcador textual inerte.

O modo didático vulnerável representa propagação insegura do dado não confiável para a mensagem de erro. O modo mitigado rejeita o tipo inválido e não reflete o valor enviado. O parser e validador são intencionalmente reduzidos, não implementações completas dos padrões HTTP ou multipart.
