#ifndef MATRIX_ITERATOR_H
#define MATRIX_ITERATOR_H

namespace mtao { 
    namespace geometry {
        namespace util {
            namespace internal {

                template <typename MatType, bool Row>//0 is col, 1 is row
                    class MatrixIterator {
                        public:
                            MatrixIterator(MatType& m, int idx=0): matRef(m), index(idx) {}
                            auto operator*() { if constexpr(Row) {return matRef.row(index);} else {return matRef.col(index);} }
                            MatrixIterator& operator++(){ index++; return *this; }
                            bool operator==(const MatrixIterator& other) const {
                                return index == other.index;
                            }
                            bool operator!=(const MatrixIterator& other) const {
                                return index != other.index;
                            }

                        private:
                            MatType& matRef;
                            int index;

                    };

                template <typename MatType, bool Row>
                    class MatrixIteratorContainer {
                        public:
                            using iterator = MatrixIterator<MatType,Row>;
                            MatrixIteratorContainer(MatType& m): matRef(m) {}
                            iterator begin() {return iterator(matRef);}
                            iterator end() {
                                if constexpr(Row) {
                                    return iterator(matRef,matRef.rows());
                                } else {
                                    return iterator(matRef,matRef.cols());
                                }
                            }
                        private:
                            MatType& matRef;
                    };
            }


            template <typename MatType>
                auto row_iterator(MatType& v, int index=0) {
                    return internal::MatrixIterator<MatType,true>(v,index);
                }

            template <typename MatType>
                auto row_iterator_container(MatType& v) {
                    return internal::MatrixIteratorContainer<MatType,true>(v);
                }

            template <typename MatType>
                auto col_iterator(MatType& v, int index=0) {
                    return internal::MatrixIterator<MatType,false>(v,index);
                }

            template <typename MatType>
                auto col_iterator_container(MatType& v) {
                    return internal::MatrixIteratorContainer<MatType,false>(v);
                }
        }}}

#endif//MATRIX_ITERATOR_H
