/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINEX_IIO_KFIFO_BUF_H__
#define __LINEX_IIO_KFIFO_BUF_H__

struct iio_buffer;
struct iio_buffer_setup_ops;
struct iio_dev;
struct iio_dev_attr;
struct device;

struct iio_buffer *iio_kfifo_allocate(void);
void iio_kfifo_free(struct iio_buffer *r);

int devm_iio_kfifo_buffer_setup_ext(struct device *dev,
				    struct iio_dev *indio_dev,
				    const struct iio_buffer_setup_ops *setup_ops,
				    const struct iio_dev_attr **buffer_attrs);

#define devm_iio_kfifo_buffer_setup(dev, indio_dev, setup_ops)	\
	devm_iio_kfifo_buffer_setup_ext((dev), (indio_dev), (setup_ops), NULL)

#endif
