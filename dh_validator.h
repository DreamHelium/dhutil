/* dh_validator.h generated by valac 0.56.3, the Vala compiler, do not modify */

#ifndef __DH_VALIDATOR_H__
#define __DH_VALIDATOR_H__

#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib-object.h>
#include <float.h>
#include <math.h>

G_BEGIN_DECLS

#if !defined(VALA_EXTERN)
#if defined(_MSC_VER)
#define VALA_EXTERN __declspec(dllexport) extern
#elif __GNUC__ >= 4
#define VALA_EXTERN __attribute__((visibility("default"))) extern
#else
#define VALA_EXTERN extern
#endif
#endif

typedef gchar* (*DhLineinFunc) (const gchar* prompt, gpointer user_data);

#define TYPE_DH_VALIDATOR (dh_validator_get_type ())
#define DH_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DH_VALIDATOR, DhValidator))
#define IS_DH_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DH_VALIDATOR))
#define DH_VALIDATOR_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), TYPE_DH_VALIDATOR, DhValidatorIface))

typedef struct _DhValidator DhValidator;
typedef struct _DhValidatorIface DhValidatorIface;

#define TYPE_DH_INT_VALIDATOR (dh_int_validator_get_type ())
#define DH_INT_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DH_INT_VALIDATOR, DhIntValidator))
#define DH_INT_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_DH_INT_VALIDATOR, DhIntValidatorClass))
#define IS_DH_INT_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DH_INT_VALIDATOR))
#define IS_DH_INT_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_DH_INT_VALIDATOR))
#define DH_INT_VALIDATOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_DH_INT_VALIDATOR, DhIntValidatorClass))

typedef struct _DhIntValidator DhIntValidator;
typedef struct _DhIntValidatorClass DhIntValidatorClass;
typedef struct _DhIntValidatorPrivate DhIntValidatorPrivate;

#define TYPE_DH_UINT_VALIDATOR (dh_uint_validator_get_type ())
#define DH_UINT_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DH_UINT_VALIDATOR, DhUIntValidator))
#define DH_UINT_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_DH_UINT_VALIDATOR, DhUIntValidatorClass))
#define IS_DH_UINT_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DH_UINT_VALIDATOR))
#define IS_DH_UINT_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_DH_UINT_VALIDATOR))
#define DH_UINT_VALIDATOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_DH_UINT_VALIDATOR, DhUIntValidatorClass))

typedef struct _DhUIntValidator DhUIntValidator;
typedef struct _DhUIntValidatorClass DhUIntValidatorClass;
typedef struct _DhUIntValidatorPrivate DhUIntValidatorPrivate;

#define TYPE_DH_DOUBLE_VALIDATOR (dh_double_validator_get_type ())
#define DH_DOUBLE_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DH_DOUBLE_VALIDATOR, DhDoubleValidator))
#define DH_DOUBLE_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_DH_DOUBLE_VALIDATOR, DhDoubleValidatorClass))
#define IS_DH_DOUBLE_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DH_DOUBLE_VALIDATOR))
#define IS_DH_DOUBLE_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_DH_DOUBLE_VALIDATOR))
#define DH_DOUBLE_VALIDATOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_DH_DOUBLE_VALIDATOR, DhDoubleValidatorClass))

typedef struct _DhDoubleValidator DhDoubleValidator;
typedef struct _DhDoubleValidatorClass DhDoubleValidatorClass;
typedef struct _DhDoubleValidatorPrivate DhDoubleValidatorPrivate;

#define TYPE_DH_REGEX_VALIDATOR (dh_regex_validator_get_type ())
#define DH_REGEX_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DH_REGEX_VALIDATOR, DhRegexValidator))
#define DH_REGEX_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_DH_REGEX_VALIDATOR, DhRegexValidatorClass))
#define IS_DH_REGEX_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DH_REGEX_VALIDATOR))
#define IS_DH_REGEX_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_DH_REGEX_VALIDATOR))
#define DH_REGEX_VALIDATOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_DH_REGEX_VALIDATOR, DhRegexValidatorClass))

typedef struct _DhRegexValidator DhRegexValidator;
typedef struct _DhRegexValidatorClass DhRegexValidatorClass;
typedef struct _DhRegexValidatorPrivate DhRegexValidatorPrivate;

#define TYPE_DH_MATCH_VALIDATOR (dh_match_validator_get_type ())
#define DH_MATCH_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DH_MATCH_VALIDATOR, DhMatchValidator))
#define DH_MATCH_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_DH_MATCH_VALIDATOR, DhMatchValidatorClass))
#define IS_DH_MATCH_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DH_MATCH_VALIDATOR))
#define IS_DH_MATCH_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_DH_MATCH_VALIDATOR))
#define DH_MATCH_VALIDATOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_DH_MATCH_VALIDATOR, DhMatchValidatorClass))

typedef struct _DhMatchValidator DhMatchValidator;
typedef struct _DhMatchValidatorClass DhMatchValidatorClass;
typedef struct _DhMatchValidatorPrivate DhMatchValidatorPrivate;

#define TYPE_DH_INT_ARRAY_VALIDATOR (dh_int_array_validator_get_type ())
#define DH_INT_ARRAY_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DH_INT_ARRAY_VALIDATOR, DhIntArrayValidator))
#define DH_INT_ARRAY_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_DH_INT_ARRAY_VALIDATOR, DhIntArrayValidatorClass))
#define IS_DH_INT_ARRAY_VALIDATOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DH_INT_ARRAY_VALIDATOR))
#define IS_DH_INT_ARRAY_VALIDATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_DH_INT_ARRAY_VALIDATOR))
#define DH_INT_ARRAY_VALIDATOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_DH_INT_ARRAY_VALIDATOR, DhIntArrayValidatorClass))

typedef struct _DhIntArrayValidator DhIntArrayValidator;
typedef struct _DhIntArrayValidatorClass DhIntArrayValidatorClass;
typedef struct _DhIntArrayValidatorPrivate DhIntArrayValidatorPrivate;

#define TYPE_DH_ARG_INFO (dh_arg_info_get_type ())
#define DH_ARG_INFO(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DH_ARG_INFO, DhArgInfo))
#define DH_ARG_INFO_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_DH_ARG_INFO, DhArgInfoClass))
#define IS_DH_ARG_INFO(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DH_ARG_INFO))
#define IS_DH_ARG_INFO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_DH_ARG_INFO))
#define DH_ARG_INFO_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_DH_ARG_INFO, DhArgInfoClass))

typedef struct _DhArgInfo DhArgInfo;
typedef struct _DhArgInfoClass DhArgInfoClass;
typedef struct _DhArgInfoPrivate DhArgInfoPrivate;

#define TYPE_DH_OUT (dh_out_get_type ())
#define DH_OUT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DH_OUT, DhOut))
#define DH_OUT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_DH_OUT, DhOutClass))
#define IS_DH_OUT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DH_OUT))
#define IS_DH_OUT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_DH_OUT))
#define DH_OUT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_DH_OUT, DhOutClass))

typedef struct _DhOut DhOut;
typedef struct _DhOutClass DhOutClass;
typedef struct _DhOutPrivate DhOutPrivate;

struct _DhValidatorIface {
	GTypeInterface parent_iface;
	gboolean (*in_field) (DhValidator* self, gconstpointer val);
	void (*set_range) (DhValidator* self, gconstpointer min_val, gconstpointer max_val);
};

struct _DhIntValidator {
	GObject parent_instance;
	DhIntValidatorPrivate * priv;
};

struct _DhIntValidatorClass {
	GObjectClass parent_class;
};

struct _DhUIntValidator {
	GObject parent_instance;
	DhUIntValidatorPrivate * priv;
};

struct _DhUIntValidatorClass {
	GObjectClass parent_class;
};

struct _DhDoubleValidator {
	GObject parent_instance;
	DhDoubleValidatorPrivate * priv;
};

struct _DhDoubleValidatorClass {
	GObjectClass parent_class;
};

struct _DhRegexValidator {
	GObject parent_instance;
	DhRegexValidatorPrivate * priv;
};

struct _DhRegexValidatorClass {
	GObjectClass parent_class;
};

struct _DhMatchValidator {
	GObject parent_instance;
	DhMatchValidatorPrivate * priv;
};

struct _DhMatchValidatorClass {
	GObjectClass parent_class;
};

struct _DhIntArrayValidator {
	GObject parent_instance;
	DhIntArrayValidatorPrivate * priv;
	gchar* split_str;
};

struct _DhIntArrayValidatorClass {
	GObjectClass parent_class;
};

struct _DhArgInfo {
	GObject parent_instance;
	DhArgInfoPrivate * priv;
	GList* arg;
	GList* arg_fullname;
	GList* description;
};

struct _DhArgInfoClass {
	GObjectClass parent_class;
};

struct _DhOut {
	GObject parent_instance;
	DhOutPrivate * priv;
};

struct _DhOutClass {
	GObjectClass parent_class;
};

VALA_EXTERN GType dh_validator_get_type (void) G_GNUC_CONST ;
G_DEFINE_AUTOPTR_CLEANUP_FUNC (DhValidator, g_object_unref)
VALA_EXTERN gboolean dh_validator_in_field (DhValidator* self,
                                gconstpointer val);
VALA_EXTERN void dh_validator_set_range (DhValidator* self,
                             gconstpointer min_val,
                             gconstpointer max_val);
VALA_EXTERN GType dh_int_validator_get_type (void) G_GNUC_CONST ;
G_DEFINE_AUTOPTR_CLEANUP_FUNC (DhIntValidator, g_object_unref)
VALA_EXTERN DhIntValidator* dh_int_validator_new_ignore_range (gboolean ir);
VALA_EXTERN DhIntValidator* dh_int_validator_construct_ignore_range (GType object_type,
                                                         gboolean ir);
VALA_EXTERN DhIntValidator* dh_int_validator_new (gint64 min,
                                      gint64 max);
VALA_EXTERN DhIntValidator* dh_int_validator_construct (GType object_type,
                                            gint64 min,
                                            gint64 max);
VALA_EXTERN GType dh_uint_validator_get_type (void) G_GNUC_CONST ;
G_DEFINE_AUTOPTR_CLEANUP_FUNC (DhUIntValidator, g_object_unref)
VALA_EXTERN DhUIntValidator* dh_uint_validator_new_ignore_range (gboolean ir);
VALA_EXTERN DhUIntValidator* dh_uint_validator_construct_ignore_range (GType object_type,
                                                           gboolean ir);
VALA_EXTERN DhUIntValidator* dh_uint_validator_new (guint64 min,
                                        guint64 max);
VALA_EXTERN DhUIntValidator* dh_uint_validator_construct (GType object_type,
                                              guint64 min,
                                              guint64 max);
VALA_EXTERN GType dh_double_validator_get_type (void) G_GNUC_CONST ;
G_DEFINE_AUTOPTR_CLEANUP_FUNC (DhDoubleValidator, g_object_unref)
VALA_EXTERN DhDoubleValidator* dh_double_validator_new_ignore_range (gboolean ir);
VALA_EXTERN DhDoubleValidator* dh_double_validator_construct_ignore_range (GType object_type,
                                                               gboolean ir);
VALA_EXTERN DhDoubleValidator* dh_double_validator_new (gdouble min,
                                            gdouble max);
VALA_EXTERN DhDoubleValidator* dh_double_validator_construct (GType object_type,
                                                  gdouble min,
                                                  gdouble max);
VALA_EXTERN GType dh_regex_validator_get_type (void) G_GNUC_CONST ;
G_DEFINE_AUTOPTR_CLEANUP_FUNC (DhRegexValidator, g_object_unref)
VALA_EXTERN DhRegexValidator* dh_regex_validator_new (GRegex* r);
VALA_EXTERN DhRegexValidator* dh_regex_validator_construct (GType object_type,
                                                GRegex* r);
VALA_EXTERN GType dh_match_validator_get_type (void) G_GNUC_CONST ;
G_DEFINE_AUTOPTR_CLEANUP_FUNC (DhMatchValidator, g_object_unref)
VALA_EXTERN DhMatchValidator* dh_match_validator_new_ignore_range (gboolean ir);
VALA_EXTERN DhMatchValidator* dh_match_validator_construct_ignore_range (GType object_type,
                                                             gboolean ir);
VALA_EXTERN DhMatchValidator* dh_match_validator_new (const gchar* str);
VALA_EXTERN DhMatchValidator* dh_match_validator_construct (GType object_type,
                                                const gchar* str);
VALA_EXTERN GType dh_int_array_validator_get_type (void) G_GNUC_CONST ;
G_DEFINE_AUTOPTR_CLEANUP_FUNC (DhIntArrayValidator, g_object_unref)
VALA_EXTERN DhIntArrayValidator* dh_int_array_validator_new_ignore_range (gboolean ir);
VALA_EXTERN DhIntArrayValidator* dh_int_array_validator_construct_ignore_range (GType object_type,
                                                                    gboolean ir);
VALA_EXTERN void dh_int_array_validator_add_range (DhIntArrayValidator* self,
                                       gint64 min_val,
                                       gint64 max_val);
VALA_EXTERN void dh_int_array_validator_set_split_str (DhIntArrayValidator* self,
                                           const gchar* sp_str);
VALA_EXTERN void dh_int_array_validator_set_allow_repeated (DhIntArrayValidator* self,
                                                gboolean ar);
VALA_EXTERN DhIntArrayValidator* dh_int_array_validator_new (void);
VALA_EXTERN DhIntArrayValidator* dh_int_array_validator_construct (GType object_type);
VALA_EXTERN GType dh_arg_info_get_type (void) G_GNUC_CONST ;
G_DEFINE_AUTOPTR_CLEANUP_FUNC (DhArgInfo, g_object_unref)
VALA_EXTERN void dh_arg_info_add_arg (DhArgInfo* self,
                          gchar new_arg,
                          const gchar* new_arg_fullname,
                          const gchar* new_description);
VALA_EXTERN gchar* dh_arg_info_help_message (DhArgInfo* self,
                                 const gchar* gettext_package);
VALA_EXTERN gchar dh_arg_info_match_char (DhArgInfo* self,
                              const gchar* str);
VALA_EXTERN gchar* dh_arg_info_match_string (DhArgInfo* self,
                                 const gchar* str);
VALA_EXTERN void dh_arg_info_change_default_arg (DhArgInfo* self,
                                     gchar arg);
VALA_EXTERN DhArgInfo* dh_arg_info_new (void);
VALA_EXTERN DhArgInfo* dh_arg_info_construct (GType object_type);
VALA_EXTERN GType dh_out_get_type (void) G_GNUC_CONST ;
G_DEFINE_AUTOPTR_CLEANUP_FUNC (DhOut, g_object_unref)
VALA_EXTERN void dh_out_set_show_opt (DhOut* self,
                          gboolean option);
VALA_EXTERN void dh_out_no_output_string_while_no_validator (DhOut* self);
VALA_EXTERN void dh_out_output_match_string_than_arg (DhOut* self);
VALA_EXTERN void dh_out_read_and_output (DhOut* self,
                             const gchar* tip_message,
                             const gchar* gettext_package,
                             DhArgInfo* arg,
                             DhValidator* validator,
                             gboolean get_array,
                             GValue* result);
VALA_EXTERN void dh_out_read_and_output_custom (DhOut* self,
                                    DhLineinFunc func,
                                    gpointer func_target,
                                    const gchar* tip_message,
                                    const gchar* gettext_package,
                                    DhArgInfo* arg,
                                    DhValidator* validator,
                                    gboolean get_array,
                                    gboolean use_readline,
                                    GValue* result);
VALA_EXTERN void dh_out_read_and_output_as_int_custom (DhOut* self,
                                           DhLineinFunc func,
                                           gpointer func_target,
                                           const gchar* tip_message,
                                           const gchar* gettext_package,
                                           DhArgInfo* arg,
                                           gint64 min,
                                           gint64 max,
                                           gboolean get_array,
                                           gboolean use_readline,
                                           GValue* result);
VALA_EXTERN void dh_out_read_and_output_as_int (DhOut* self,
                                    const gchar* tip_message,
                                    const gchar* gettext_package,
                                    DhArgInfo* arg,
                                    gint64 min,
                                    gint64 max,
                                    gboolean get_array,
                                    GValue* result);
VALA_EXTERN DhOut* dh_out_new (void);
VALA_EXTERN DhOut* dh_out_construct (GType object_type);

G_END_DECLS

#endif
