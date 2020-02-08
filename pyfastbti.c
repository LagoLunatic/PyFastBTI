#define PY_SSIZE_T_CLEAN
#include <Python.h>

int pyfastbti_get_color_distance_fast(int r1, int g1, int b1, int r2, int g2, int b2) {
  int dist = 0;
  dist += abs(r1 - r2);
  dist += abs(g1 - g2);
  dist += abs(b1 - b2);
  return dist;
}

static PyObject* pyfastbti_get_best_cmpr_key_colors(PyObject* self, PyObject* args) {
  PyObject* src_color_tuples;
  int num_src_colors;
  int* src_colors;
  int max_dist = -1;
  int color_1[4];
  int color_2[4];
  color_1[3] = 0xFF;
  color_2[3] = 0xFF;
  
  if (!PyArg_ParseTuple(args, "O", &src_color_tuples)) {
    return NULL; // Error already raised
  }
  src_color_tuples = PySequence_Fast(src_color_tuples, "Argument must be iterable.");
  if (!src_color_tuples) {
    return NULL; // Error already raised
  }
  
  num_src_colors = (int)PySequence_Fast_GET_SIZE(src_color_tuples);
  
  src_colors = malloc(num_src_colors*4*sizeof(int));
  if(!src_colors) {
    Py_DECREF(src_color_tuples);
    return PyErr_NoMemory();
  }
  
  // Convert the list of tuples of numbers into a 2D array of ints
  for (int i = 0; i < num_src_colors; i++) {
    PyObject *color_tuple = PySequence_Fast_GET_ITEM(src_color_tuples, i);
    if(!color_tuple) {
      Py_DECREF(src_color_tuples);
      free(src_colors);
      return 0;
    }
    
    for (int j = 0; j < 4; j++) { // RGBA
      PyObject* val_obj = PyTuple_GetItem(color_tuple, j);
      if (val_obj == NULL) {
        Py_DECREF(src_color_tuples);
        free(src_colors);
        return NULL;
      }
      if (PyNumber_Check(val_obj) != 1) {
        Py_DECREF(src_color_tuples);
        free(src_colors);
        PyErr_SetString(PyExc_TypeError, "Color tuple contains non-numeric object.");
        return NULL;
      }
      
      PyObject* val_num = PyNumber_Long(val_obj);
      src_colors[i*4+j] = PyLong_AsUnsignedLong(val_num);
      Py_DECREF(val_num);
    }
  }
  
  for (int i = 0; i < num_src_colors; i++) {
    int r1 = src_colors[i*4+0];
    int g1 = src_colors[i*4+1];
    int b1 = src_colors[i*4+2];
    
    for (int j = i+1; j < num_src_colors; j++) {
      int r2 = src_colors[j*4+0];
      int g2 = src_colors[j*4+1];
      int b2 = src_colors[j*4+2];
      
      int curr_dist = pyfastbti_get_color_distance_fast(r1, g1, b1, r2, g2, b2);
      
      if (curr_dist > max_dist) {
        max_dist = curr_dist;
        color_1[0] = r1;
        color_1[1] = g1;
        color_1[2] = b1;
        color_2[0] = r2;
        color_2[1] = g2;
        color_2[2] = b2;
      }
    }
  }
  
  if (max_dist == -1) {
    color_1[0] = 0x00;
    color_1[1] = 0x00;
    color_1[2] = 0x00;
    color_2[0] = 0xFF;
    color_2[1] = 0xFF;
    color_2[2] = 0xFF;
  } else {
    int r1 = color_1[0];
    int g1 = color_1[1];
    int b1 = color_1[2];
    int r2 = color_2[0];
    int g2 = color_2[1];
    int b2 = color_2[2];
    
    if ((r1 >> 3) == (r2 >> 3) && (g1 >> 2) == (g2 >> 2) && (b1 >> 3) == (b2 >> 3)) {
      if ((r1 >> 3) == 0 && (g1 >> 2) == 0 && (b1 >> 3) == 0) {
        color_2[0] = 0xFF;
        color_2[1] = 0xFF;
        color_2[2] = 0xFF;
      } else {
        color_2[0] = 0x00;
        color_2[1] = 0x00;
        color_2[2] = 0x00;
      }
    }
  }
  
  PyObject* color_1_tuple = Py_BuildValue("(iiii)", color_1[0], color_1[1], color_1[2], color_1[3]);
  PyObject* color_2_tuple = Py_BuildValue("(iiii)", color_2[0], color_2[1], color_2[2], color_2[3]);
  
  Py_DECREF(src_color_tuples);
  free(src_colors);
  return Py_BuildValue("(OO)", color_1_tuple, color_2_tuple);
}

static PyMethodDef pyfastbtiMethods[] = {
  {"get_best_cmpr_key_colors", pyfastbti_get_best_cmpr_key_colors, METH_VARARGS, "Takes a list of colors as RGBA tuples and returns a tuple containing the two colors that make the best key colors for CMPR compression."},
  {NULL, NULL, 0, NULL} // Sentinel
};

static struct PyModuleDef pyfastbti_module = {
  PyModuleDef_HEAD_INIT,
  "pyfastbti", // Module name
  NULL, // Documentation
  -1, // Size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
  pyfastbtiMethods
};

PyMODINIT_FUNC PyInit_pyfastbti(void) {
  PyObject* module;
  
  module = PyModule_Create(&pyfastbti_module);
  if (module == NULL) {
    return NULL;
  }
  
  return module;
}
