package com.pratik.window;

// Matric Package
import android.opengl.Matrix;

public class Stack {

    public final static int MODEL_VIEW_MATRIX_STACK = 32;
    static float matrixStack[][] = new float[MODEL_VIEW_MATRIX_STACK][16]; //
    static int matrixStackTop = -1;

    static void initializeMatrixStack() {
        // code
        matrixStackTop = 0;
        for (int i = matrixStackTop; i < MODEL_VIEW_MATRIX_STACK; i++)
            Matrix.setIdentityM(matrixStack[i], 0);
    }

    static void pushMatrix(float[] matrix) {

        // Code
        System.out.println(
                "PRJ: Before Push Stack Top :" + matrix[0] + " " + matrix[1] + " " + matrix[2] + " " + matrix[3]);

        if (matrixStackTop > (MODEL_VIEW_MATRIX_STACK - 1)) {
            System.out.println("PRJ: ERROR - EXCEEDED MATRIX STACK LIMIT :");
            return;
            // finalize();
        }
        // matrixStack[matrixStackTop] = matrix;

        matrixStack[matrixStackTop] = matrix.clone();

        // for (int i = 0; i < matrix.length; i++) {
        // matrixStack[matrixStackTop][i] = matrix[i];
        // }

        System.out.println(
                "PRJ: After Push Stack To :" + matrixStack[matrixStackTop][0] + " " + matrixStack[matrixStackTop][1]
                        + " " +
                        matrixStack[matrixStackTop][2] + " " + matrixStack[matrixStackTop][3]);

        matrixStackTop++;
    }

    static float[] popMatrix() {

        // variable declartions
        float[] matrix = new float[16];

        // code
        // System.out.println("PRJ: Before Pop Staqck Top :" + matrixStackTop);

        if (matrixStackTop < 0) {
            System.out.println("PRJ: ERROR : MATRIX STACK EMPTY!");
            // finalize();
        }

        Matrix.setIdentityM(matrixStack[matrixStackTop], 0);

        matrixStackTop--;

        // matrix = matrixStack[matrixStackTop].clone();
        matrix = matrixStack[matrixStackTop].clone();

        // System.out.println("PRJ: After Pop Staqck Top = :" + matrixStackTop);

        return (matrix);
    }
    /*
     * static void finalize() {
     * System.out.println("Object is destroyed by the Garbage Collector");
     * }
     */
}
