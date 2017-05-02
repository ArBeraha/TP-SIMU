# Trabajo Práctico de Simulación
| Título del trabajo | Simulación de un consultorio médico |
| --- | --- |

Nuestro sistema se basa en un consultorio médico. Las personas que se presentan puede que sea para atenderse espontáneamente con un médico o para realizar un trámite con un secretario del mismo. Se sabe que un 70%pasa a la cola de médicos, y el resto es atendido por un secretario. 

Este intervalo de arribos responde a la siguiente f.d.p: **f(x)=1/5.81775 e^((-x-5.7099)/5.81775-e^((-x-5.7099)/5.81775) )**

El horario de atención del consultorio es de 15 a 20hs. Luego de las 20 hs ya no ingresa gente al sistema y se debe despachar a la gente hasta que no quede nadie.
La gente que espera para ver un médico no se arrepiente. Los que esperan para ser atendidos por el secretario, el 30% se arrepienten si ve más de 5.A las personas esperando en la cola del medico las atenderá cualquier profesional que este desocupado. Si todos los médicos están desocupados van a cualquiera. Lo mismo sucede en la cola de los secretarios, los que esperan en la cola de un secretario serán atendidos por cualquier secretario libre y el caso de que varios estén libres cualquiera podrá atenderlos.

El tiempo de atención depende de lo que se tenga que realizar el paciente, teniendo datos:El médico tarda siempre entre 15 y 30 minutos, su tiempo de atención viene dado por una f.d.p aleatoria, equiprobable entre esos dos valores. El secretario realiza las siguientes tareas:
- Recibe sobre de recetas.
- Entrega sobre de recetas.
- Realiza recetas electrónicas en el momento.
- Duplicado de recetas.

El tiempo de atención del secretario puede variar entre 3 a 40 minutos y viene dado por la siguiente f.d.p: **f(x)=1/12.394 (1+0.38114((x-2.0125)/12.394))^(-1+1/0.38114) **


Cuando un médico termina de atender el 30% de las personas pasan a una nueva cola esperando que los atienda un secretario para realizar algún trámite con el mismo. Esta cola tiene prioridad para el secretario. Cada vez que un secretario termina de atender una persona que solo venia a hacer un trámite, debe chequear que no tenga nadie en cola de las personas salientes de consultar con un médico.

Nuestro objetivo es analizar los resultados con este modelo para posteriormente realizar ensayos modificando la cantidad de Médicos y la cantidad de Secretarios para estudiar un poco más en detalle lo que pasaría si incrementamos los puestos de atención con las restricciones que nos brinda el problema.
