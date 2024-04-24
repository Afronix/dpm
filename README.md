# dpm - Debian Package Manager for RPM-based Systems

O dpm é uma ferramenta inteligente para instalar pacotes Debian em sistemas baseados em RPM, oferecendo uma solução simples para a instalação de software em distribuições Linux que utilizam o formato RPM. Com o dpm, você pode desfrutar da vasta gama de pacotes disponíveis para distribuições Debian em seu sistema RPM, sem complicações.

## Recursos Principais

- **Instalação Facilitada:** Instale pacotes Debian (formato .deb) em sistemas RPM com apenas alguns comandos simples.
- **Resolução Automática de Dependências:** O dpm resolve automaticamente as dependências necessárias para garantir uma instalação sem problemas.
- **Interface Intuitiva:** Uma interface amigável torna a instalação de software uma tarefa fácil e sem complicações.
- **Interoperabilidade:** Promove a interoperabilidade entre diferentes distribuições Linux, permitindo que usuários de sistemas RPM tenham acesso a um amplo conjunto de software desenvolvido para distribuições Debian.

## Como Usar

1. **Clone o Repositório:**
```shell
git clone https://github.com/Afronix/dpm/
```
2. **Altere as permissões**
```shell
sudo chmod +x -R dpm
```
3. **Execute o Script de Instalação:**
```shell
cd dpm
sudo ./install.sh
```
4. **Execute o Programa:**
```shell
sudo dpm [arquivo.deb]
```


Se preferir não instalar o dpm no seu sistema, você também pode executá-lo diretamente do projeto clonado. Basta navegar até o diretório do projeto e usar o script principal:
```shell
cd dpm
sudo ./dpm [arquivo.deb]
```

Essa adição fornece aos usuários uma alternativa para usar o dpm sem a necessidade de instalação explícita no sistema.


## Contribuições

Contribuições são bem-vindas! Se você tem ideias para melhorar o dpm, sinta-se à vontade para enviar pull requests, relatar problemas ou sugerir novos recursos.

## Licença

Este projeto é licenciado sob a [Licença MIT](https://opensource.org/licenses/MIT) - consulte o arquivo LICENSE para obter detalhes.

---

**Nota:** O dpm é uma ferramenta em desenvolvimento. Use-a com cuidado e esteja ciente de possíveis limitações ou problemas. Se encontrar algum problema, não hesite em relatar para que possamos melhorar o projeto.
