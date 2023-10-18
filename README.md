# Hipociclóide

### Humberto Turioni Marinho - RA: 11201722439 (Turma A)
### Pedro Moreno Silva - RA: 11201722110 (Turma B)
Primeiro projeto ministrado para a disciplina  [MCTA008-17 Computação Gráfica](http://professor.ufabc.edu.br/~harlen.batagelo/cg/) na [UFABC](https://www.ufabc.edu.br/) (período 2023.3) usando a biblioteca [ABCg](https://github.com/hbatagelo/abcg) e [OpenGL](https://www.opengl.org).


## Instalação

1. Siga as instruções de instalação e montagem no [site da disciplina](https://hbatagelo.github.io/abcg/abcg/doc/html/).
2. Faça o build e execute o arquivo `hipocicloide` em `build/bin/hipocicloide`.

Alternativamente, o programa também está hospedado [nesta página web](https://humberto-marinho.github.io/Projeto1PedroHumberto/public/index).


## Sobre o projeto

**Hipociclóide** é uma aplicação em que podemos visualizar a formação de diversas curvas das classes das [hipociclóides](https://pt.wikipedia.org/wiki/Hipocicloide). Ao definirmos um círculo principal, maior, e um círculo interno, menor, tangente a parte interior do círculo maior, e o deslizarmos, formamos formas "em formato de estrela" muito interessantes e diversas.

Nos baseamos na aula do [triângulo de Sierpinksi](https://hbatagelo.github.io/cg/sierpinski.html). Para fazer a animação do desenho das formas, desenhamos um ponto por vez com a diretiva `GL_POINTS`, e para que o desenho persista, desativamos o _double buffering_ para o que o _backbuffer_ não seja apagado a cada quadro.

Para dar variedade às formas que podem ser geradas, o usuário pode modificar os parâmetros $R$, $r$ e $d$. $R$ corresponde ao raio do círculo maior e $r$, o raio do círculo menor. Já $d$ corresponde a um _offset_ entre o centro do menor círculo e o ponto que será desenhado. Se $d > r$, o ponto a ser desenhado fica fora do círculo menor, o que por sua vez gera "laços" na ponta das hipociclóides, e a curva passa a ser uma hipociclóide _alongada_. Por sua vez, se $d < r$, o ponto a ser desenhado fica dentro do círculo menor, e a hipociclóide formada tem suas pontas "achatadas", formando uma hipociclóide _encurtada_. Além disso, o usuário pode modificar a "velocidade" do traçado do desenho, que nada mais é do que aumentar ou diminuir o incremento da variável das equações paramétricas da curva. Também há a opção de mudar a cor de fundo e de parar ou continuar o traçado do desenho.

Uma consequência um tanto indesejada de desligar o _double buffering_ é que toda vez que desejamos aplicar alguma modificação em parâmetros ou cor de fundo, temos que limpar a tela. Do contrário, as mudanças não tomam efeito.

