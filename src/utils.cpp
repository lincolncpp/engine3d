// funcao para multiplicar duas "matrizes 4x4" (na realidade sao vetores de 16 elementos)
void multiplica(float *m1, float *m2, float *m_resultado){

    // OpenGL lida recebe vetores de 16 elementos e interpreta como matrizes 4x4.
    // Nessa funcao, transformamos as matrizes de volta para float[4][4] para facilitar a multiplicacao

    float m_a[4][4];
    float m_b[4][4];
    float m_c[4][4]; // m_c = m_a * m_b

    int n = 0;
    for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
            m_a[i][j] = m1[n];
            m_b[i][j] = m2[n];
            n += 1;
        }
    }


    for(int i=0;i<4;i++){    
        for(int j=0;j<4;j++){
            m_c[i][j]=0.0;
            for(int k=0; k < 4; k++){
                m_c[i][j] += m_a[i][k]*m_b[k][j];    
            }
        }
    }

    // voltando a resposta para o formato do OpenGL
    n = 0;
    for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
            m_resultado[n] = m_c[i][j];
            // printf("%f \t",m_c[i][j]);
            n += 1;
        }
        // printf("\n");
    }
    // printf("-----------------------------------\n");


}