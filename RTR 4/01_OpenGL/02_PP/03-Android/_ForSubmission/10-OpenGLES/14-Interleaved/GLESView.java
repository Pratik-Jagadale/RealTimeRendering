package com.pratik.window;

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;

import javax.microedition.khronos.egl.EGLConfig;
import android.opengl.GLES32;

import android.content.Context;

import android.graphics.Color;

import android.view.Gravity;

// Event Related Packages
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;

import java.nio.ByteBuffer; // nio = native I/O or non blocking I/O (asyncronous)
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

// Matric Package
import android.opengl.Matrix;

// Sphere Relate
import java.nio.ShortBuffer;
import java.sql.PreparedStatement;

/* Texture Related Imports */
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

public class GLESView extends GLSurfaceView implements OnDoubleTapListener, OnGestureListener, GLSurfaceView.Renderer {

    // Properties
    private GestureDetector gestureDetector;
    private Context context;
    private int shaderProgramObject;

    private int vao_Cube[] = new int[1]; // Vertex Array Object - Cube
    private int vbo_Cube[] = new int[1]; // Vertex Buffer Object - Cube

    private int modelMatrixUniform;
    private int viewMatrixUniform;
    private int ProjectionMatrixUniform;
    private int textureSamplerUniform;

    private float perspectiveProjectionMatrix[] = new float[16];

    private int laUniform;
    private int ldUniform; // light direction
    private int lsUniform; // light Spec
    private int lightPositionUniform;

    private int kaUniform; // material amb
    private int kdUniform; // material diffuse
    private int ksUniform; // mat specular
    private int materialShininessUniform;

    private int lightingEnabledUniform;
    private int lightingEnabledUniform_PV;

    private float lightAmbiant[] = new float[] { 0.1f, 0.1f, 0.1f, 1.0f };
    private float lightDiffuse[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float lightSpecular[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float lightPosition[] = new float[] { 0.0f, 0.0f, 10.0f, 1.0f };

    private float materialAmbiant[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    private float materialDiffuse[] = new float[] { 0.5f, 0.2f, 0.7f, 1.0f };
    private float materialSpecular[] = new float[] { 0.7f, 0.7f, 0.7f, 1.0f };
    private float materialShineeness = 120.0f;

    private Boolean bLight = false;

    private int texture_marble[] = new int[1];
    private float angleCube = 0.0f;

    // Beahviours
    GLESView(Context _context) {
        super(_context);

        context = _context;

        setEGLContextClientVersion(3);

        setRenderer(this);

        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        // Gesture Related Code
        gestureDetector = new GestureDetector(context, this, null, false);
        gestureDetector.setOnDoubleTapListener(this);
    }

    // Three Methods of GLSurfaceView.Renderer
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        String glesVersion = gl.glGetString(GL10.GL_VERSION);
        System.out.println("PRJ: " + glesVersion);

        String renderer = gl.glGetString(GL10.GL_RENDERER);
        System.out.println("PRJ: " + renderer);

        String glslVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
        System.out.println("PRJ: " + glslVersion);

        initialize();
    }

    @Override
    public void onSurfaceChanged(GL10 unUsed, int width, int height) {
        resize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 unUsed) {
        // onDrawFrame should be consider as GameLoop
        update();
        display();
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        // Code
        if (!gestureDetector.onTouchEvent(e))
            super.onTouchEvent(e);

        return true;
    }

    // Three methods from OnDoubleTapListaner Interface
    @Override
    public boolean onDoubleTap(MotionEvent e) {
        return true;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        if (bLight == true) {
            bLight = false;
        } else {
            bLight = true;
        }
        return true;
    }

    // Six Methods of onGestureListaner
    @Override
    public boolean onDown(MotionEvent e) {
        return true;
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e) {
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        uninitialize();
        System.exit(0);
        return true;
    }

    @Override
    public void onShowPress(MotionEvent e) {
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return true;
    }

    // Custom Private Function
    private void initialize() {

        // Vertex Shader
        final String vertexShaderSourceCode = String.format(
                "#version 320 es" +
                        "\n" +
                        "in vec4 a_position;" +
                        "in vec4 a_color;" +
                        "in vec3 a_normal;" +
                        "in vec2 a_texcoord;" +
                        "uniform mat4 u_modelMatrix;" +
                        "uniform mat4 u_viewMatrix;" +
                        "uniform mat4 u_projectionMatrix;" +
                        "uniform vec4 u_lightPosition;" +
                        "uniform mediump int u_lightingEnabled;" +
                        "out vec3 transformedNormals;" +
                        "out vec3 lightDirection;" +
                        "out vec3 viewerVector;" +
                        "out vec4 a_color_out;" +
                        "out vec2 a_texcoord_out;" +
                        "void main(void)" +
                        "{" +
                        "if(u_lightingEnabled == 1)" +
                        "{" +
                        "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
                        "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
                        "transformedNormals = normalMatrix * a_normal;" +
                        "lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;" + // Swizaling+
                        "viewerVector = -eyeCoordinates.xyz;" +
                        "}" +
                        "gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" +
                        "a_color_out = a_color;" +
                        "a_texcoord_out = a_texcoord;" +
                        "}");

        int vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);

        GLES32.glCompileShader(vertexShaderObject);

        int status[] = new int[1];

        int infoLogLength[] = new int[1];

        String log = null;

        GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(vertexShaderObject);
                System.out.println("PRJ: VERTEX SHADER COMPILATION LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Fragment Shader
        final String fragmentShaderSourceCode = String.format(
                "#version 320 es" +
                        "\n" +
                        "precision highp float;" +

                        "in vec3 transformedNormals;" +
                        "in vec3 lightDirection;" +
                        "in vec3 viewerVector;" +
                        "in vec4 a_color_out;" +
                        "in vec2 a_texcoord_out;" +

                        "uniform vec3 u_la;" +
                        "uniform vec3 u_ld;" +
                        "uniform vec3 u_ls;" +
                        "uniform vec3 u_ka;" +
                        "uniform vec3 u_ks;" +
                        "uniform vec3 u_kd;" +

                        "uniform highp float u_materialShininnes;" +
                        "uniform mediump int u_lightingEnabled;" +
                        "uniform highp sampler2D u_textureSampler;" +

                        "out vec4 FragColor;" +
                        "void main(void)" +
                        "{" +
                        "vec3 phong_ads_color;" +
                        "vec3 tex;" +
                        "tex = vec3(texture(u_textureSampler, a_texcoord_out));" +
                        "if(u_lightingEnabled == 1)" +
                        "{" +
                        "vec3 ambiant = u_la * u_ka;" +
                        "vec3 normalized_transformed_normals = normalize(transformedNormals);" +
                        "vec3 normalized_light_direction = normalize(lightDirection);" +
                        "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction ,normalized_transformed_normals),0.0);"
                        +
                        "vec3 reflectionVector = reflect(-normalized_light_direction,normalized_transformed_normals);" +
                        "vec3 normalized_view_vector = normalize(viewerVector);" +
                        "vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector , normalized_view_vector), 0.0), u_materialShininnes);"
                        +
                        "phong_ads_color = ambiant + diffuse + specular;" +
                        "}" +
                        "else" +
                        "{" +
                        "phong_ads_color = vec3(1.0 , 1.0 , 1.0);" +
                        "}" +
                        "FragColor = vec4(tex * phong_ads_color * vec3(a_color_out), 1.0);" +
                        "}");

        int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);

        GLES32.glCompileShader(fragmentShaderObject);

        status[0] = 0;

        infoLogLength[0] = 0;

        log = null;

        GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(fragmentShaderObject);
                System.out.println("PRJ: FRAGMENT SHADER COMPILATION LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // shader program
        shaderProgramObject = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
        GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

        // Pre-Link
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.PRJ_ATTRIBUTE_POSITION, "a_position");
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.PRJ_ATTRIBUTE_COLOR, "a_color");
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.PRJ_ATTRIBUTE_NORMAL, "a_normal");
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.PRJ_ATTRIBUTE_TEXTURE0, "a_texcoord");

        GLES32.glLinkProgram(shaderProgramObject);

        status[0] = 0;

        infoLogLength[0] = 0;

        log = null;

        GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(shaderProgramObject);
                System.out.println("PRJ: SHADER PROGRAM LINK LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Post Link
        ProjectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_modelMatrix");

        laUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_la");
        ldUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld");
        lsUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ls");
        lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition");

        kaUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ka");
        kdUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_kd");
        ksUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ks");

        materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_materialShininnes");

        lightingEnabledUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

        // cubepcnt DATA
        final float cubepcnt[] = {
                // front //color-red //normal-front //texture-front
                1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

                // right //color-green //normal-right //texture-right
                1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                // back //color-blue //normal-back //texture-back
                -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

                // left //color-cyan //normal-left //texture-back
                -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                // top //color-magenta //normal-top //texture-top
                1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

                // bottom //color-yellow //normal-bottom //texture-bottom
                1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f };

        // vao
        GLES32.glGenVertexArrays(1, vao_Cube, 0);
        GLES32.glBindVertexArray(vao_Cube[0]);

        // position vbo
        GLES32.glGenBuffers(1, vbo_Cube, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Cube[0]);

        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cubepcnt.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer floatBuffer = byteBuffer.asFloatBuffer();
        floatBuffer.put(cubepcnt);
        floatBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
                cubepcnt.length * 4,
                floatBuffer,
                GLES32.GL_STATIC_DRAW);

        // Vertex DATA
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATTRIBUTE_POSITION,
                3,
                GLES32.GL_FLOAT,
                false, 11 * 4, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATTRIBUTE_POSITION);

        // COLOR DATA
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATTRIBUTE_COLOR,
                3,
                GLES32.GL_FLOAT,
                false, 11 * 4, (3 * 4));

        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATTRIBUTE_COLOR);

        // NORMAL DATA
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATTRIBUTE_NORMAL,
                3,
                GLES32.GL_FLOAT,
                false, 11 * 4, (6 * 4));

        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATTRIBUTE_NORMAL);

        // TEX COORD
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATTRIBUTE_TEXTURE0,
                3,
                GLES32.GL_FLOAT,
                false, 11 * 4, (9 * 4));

        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATTRIBUTE_TEXTURE0);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
        GLES32.glBindVertexArray(0);

        // ---------------------------------------------------------------------------

        // Depth Related Changes
        GLES32.glClearDepthf(1.0f); // removed because not working in pp
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);
        GLES32.glEnable(GLES32.GL_CULL_FACE);

        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        GLES32.glEnable(GLES32.GL_TEXTURE_2D);

        texture_marble[0] = loadGLTexture(R.raw.marble);

        // Initialization of Project MNatrix
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);

    }

    private void resize(int width, int height) {
        if (height <= 0)
            height = 1;

        GLES32.glViewport(0, 0, width, height);

        Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float) width / (float) height, 0.1f, 100.0f);

    }

    private void update() {
        angleCube = angleCube + 1.0f;
        if (angleCube >= 360.0f)
            angleCube = angleCube - 360.0f;
    }

    private void display() {

        // VARIABLE DECLARTIONS
        float modelMatrix[] = new float[16];
        float viewMatrix[] = new float[16];
        float rotationMatrix[] = new float[16];
        float rotationMatrix_X[] = new float[16];
        float rotationMatrix_Y[] = new float[16];
        float rotationMatrix_Z[] = new float[16];
        float translateMatrix[] = new float[16];

        // CODE
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        GLES32.glUseProgram(shaderProgramObject);

        Matrix.setIdentityM(viewMatrix, 0);
        Matrix.setIdentityM(modelMatrix, 0);
        Matrix.setIdentityM(rotationMatrix_X, 0);
        Matrix.setIdentityM(rotationMatrix_Y, 0);
        Matrix.setIdentityM(rotationMatrix_Z, 0);
        Matrix.setIdentityM(rotationMatrix, 0);
        Matrix.setIdentityM(translateMatrix, 0);

        Matrix.translateM(translateMatrix, 0, 0.0f, 0.0f, -5.0f);
        Matrix.rotateM(rotationMatrix_X, 0, angleCube, 1.0f, 0.0f, 0.0f);
        Matrix.rotateM(rotationMatrix_Y, 0, angleCube, 0.0f, 1.0f, 0.0f);
        Matrix.rotateM(rotationMatrix_Z, 0, angleCube, 0.0f, 0.0f, 1.0f);

        Matrix.multiplyMM(rotationMatrix_Z, 0, rotationMatrix_Y, 0, rotationMatrix_Z, 0);
        Matrix.multiplyMM(rotationMatrix, 0, rotationMatrix_X, 0, rotationMatrix_Z, 0);
        Matrix.multiplyMM(modelMatrix, 0, translateMatrix, 0, rotationMatrix, 0);

        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);

        if (bLight == true) {

            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lightPositionUniform, 1, lightPosition, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShineeness);

        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);

        }

        GLES32.glActiveTexture(GLES32.GL_TEXTURE0); //
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture_marble[0]);
        GLES32.glUniform1i(textureSamplerUniform, 0); //

        GLES32.glUniform1i(textureSamplerUniform, 0); //
        GLES32.glBindVertexArray(vao_Cube[0]);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);

        GLES32.glBindVertexArray(0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

        GLES32.glUseProgram(0);
        requestRender();
    }

    private void uninitialize() {
        // code
        // destroy element vbo

        if (shaderProgramObject > 0) // means not equal to
        {
            GLES32.glUseProgram(shaderProgramObject);

            int retVal[] = new int[1];
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_ATTACHED_SHADERS, retVal, 0);
            if (retVal[0] > 0) {
                int numAttachedShaders = retVal[0];

                int shaderObjects[] = new int[numAttachedShaders];
                GLES32.glGetAttachedShaders(shaderProgramObject, numAttachedShaders, retVal, 0, shaderObjects, 0);

                for (int i = 0; i < numAttachedShaders; i++) {
                    GLES32.glDetachShader(shaderProgramObject, shaderObjects[i]);
                    GLES32.glDeleteShader(shaderObjects[i]);
                    shaderObjects[i] = 0;
                }
            }

            GLES32.glDeleteProgram(shaderProgramObject);
            shaderProgramObject = 0;

            GLES32.glUseProgram(0);
        }

    }

    private int loadGLTexture(int imageResourceID) {

        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inScaled = false;

        Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(), imageResourceID, options);

        int texture[] = new int[1];

        GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 1);
        GLES32.glGenTextures(1, texture, 0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture[0]);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR_MIPMAP_LINEAR);

        // create the texture
        GLUtils.texImage2D(GLES32.GL_TEXTURE_2D // target
                , 0 // Mipmap Level
                , bitmap // image data
                , 0 // Border Widtgh
        );

        GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D); // Generate MipMap
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0); // unbind texture

        return texture[0];
    }

}
